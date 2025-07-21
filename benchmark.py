#!/usr/bin/env python3
"""
compare_bench.py – interleaved benchmark for two chess engines.

UI  : single rich progress‑bar (1 tick = both engines ran once) + live table.
Extra: --high-priority   → raise engine process priority (reduces OS noise).

Example
-------
    python compare_bench.py ./andobot_old.exe ./andobot.exe 20 \
        --csv bench.csv --high-priority
"""

from __future__ import annotations

import argparse
import csv
import platform
import re
import statistics
import subprocess
import sys
from pathlib import Path
from typing import List, Tuple

from rich.console import Console, Group
from rich.live import Live
from rich.progress import (
    Progress,
    BarColumn,
    TextColumn,
    TimeElapsedColumn,
    TimeRemainingColumn,
)
from rich.table import Table

# ─────────── global objects ────────────
RE_MBOARDS = re.compile(r"([0-9]*\.?[0-9]+)\s+Million boards/s", re.IGNORECASE)
console = Console()

try:
    import psutil  # type: ignore
except ImportError:  # psutil is optional
    psutil = None


# ─────────── helpers ────────────
def _boost_priority(pid: int) -> None:
    """Attempt to give `pid` higher priority (niceness / scheduling class)."""
    if psutil is None:
        return  # silently skip if library not present
    try:
        proc = psutil.Process(pid)
        if platform.system() == "Windows":
            proc.nice(psutil.HIGH_PRIORITY_CLASS)  # type: ignore[attr-defined]
        else:  # Unix: lower nice value = higher prio;  -10 is reasonable & non‑root
            proc.nice(-10)  # type: ignore[arg-type]
    except Exception as exc:  # noqa: BLE001
        console.print(f"[yellow]Warning:[/] Couldn’t raise priority: {exc}")


def run_once(engine: Path, high_prio: bool = False) -> float:
    """Run '<engine> benchmark' and return deepest finite “Million boards/s”."""
    # Popen to allow priority tweak before it runs long
    try:
        proc = subprocess.Popen(
            [str(engine), "benchmark"],
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
    except FileNotFoundError:
        console.print(f"[bold red]ERROR:[/bold red] executable not found: {engine}")
        sys.exit(1)

    if high_prio:
        _boost_priority(proc.pid)

    stdout, stderr = proc.communicate()

    if proc.returncode:
        console.print(
            f"[bold red]ERROR:[/bold red] {engine} exited with {proc.returncode}"
        )
        console.print(stderr)
        sys.exit(1)

    mbps: List[float] = []
    num = re.compile(r"^[0-9]*\.?[0-9]+$")
    for line in stdout.splitlines():
        if not line.strip() or not line.lstrip()[0].isdigit():
            continue
        parts = line.split()
        if len(parts) >= 4 and num.match(parts[0]):  # depth column looks OK
            val = parts[3]
            if val.lower() != "inf" and num.match(val):
                mbps.append(float(val))

    if not mbps:
        console.print(
            f"[bold red]ERROR:[/bold red] could not parse benchmark output of {engine}"
        )
        sys.exit(1)

    return mbps[-1]  # deepest depth


def build_table(
    done: int,
    total: int,
    eng1: Path,
    eng2: Path,
    res1: List[float],
    res2: List[float],
) -> Table:
    """Return table with latest run and running averages."""
    table = Table(
        title=f"Interleaved Benchmark – Round {done}/{total}",
        expand=True,
        show_lines=False,
    )
    table.add_column("Engine", style="bold")
    table.add_column("Last (Mboards/s)", justify="right")
    table.add_column("Average", justify="right")

    last1 = f"{res1[-1]:.3f}" if res1 else "—"
    last2 = f"{res2[-1]:.3f}" if res2 else "—"
    avg1 = f"{statistics.mean(res1):.3f}" if res1 else "—"
    avg2 = f"{statistics.mean(res2):.3f}" if res2 else "—"

    table.add_row(str(eng1), last1, avg1)
    table.add_row(str(eng2), last2, avg2)

    if res1 and res2:
        diff = statistics.mean(res1) - statistics.mean(res2)
        pct = abs(diff) / statistics.mean(res2) * 100 if statistics.mean(res2) else 0
        faster = eng1 if diff > 0 else eng2
        table.caption = (
            f"[bold]{faster} leading by {abs(diff):.3f} Mb/s (≈{pct:.2f} %).[/bold]"
        )

    return table


# ─────────── main ────────────
def main() -> None:
    ap = argparse.ArgumentParser(description="Interleaved engine benchmark (rich UI)")
    ap.add_argument("engine1", type=Path)
    ap.add_argument("engine2", type=Path)
    ap.add_argument("runs", type=int, help="Number of interleaved rounds")
    ap.add_argument("--csv", metavar="FILE", help="Write per‑run CSV log")
    ap.add_argument(
        "--high-priority",
        action="store_true",
        help="Raise engine process priority (needs psutil; may require admin rights)",
    )
    args = ap.parse_args()

    res1: List[float] = []
    res2: List[float] = []
    csv_rows: List[Tuple[int, str, float]] = []

    progress = Progress(
        TextColumn("[progress.description]{task.description}"),
        BarColumn(bar_width=None),
        TextColumn("{task.completed}/{task.total}"),
        TimeElapsedColumn(),
        TimeRemainingColumn(),
        console=console,
    )
    task = progress.add_task("Benchmark rounds", total=args.runs)

    with Live(Group(progress), refresh_per_second=10, console=console) as live:
        for rnd in range(1, args.runs + 1):
            v1 = run_once(args.engine1, args.high_priority)
            v2 = run_once(args.engine2, args.high_priority)
            res1.append(v1)
            res2.append(v2)
            csv_rows.extend(
                (
                    (rnd, str(args.engine1), v1),
                    (rnd, str(args.engine2), v2),
                )
            )

            progress.advance(task)  # ONE tick per pair
            live.update(build_table(rnd, args.runs, args.engine1, args.engine2, res1, res2))

    # ── final summary ──
    avg1, avg2 = statistics.mean(res1), statistics.mean(res2)
    diff = avg1 - avg2
    pct = abs(diff) / avg2 * 100 if avg2 else float("inf")

    console.rule("[bold]Final Averages[/bold]")
    console.print(f"[cyan]{args.engine1}[/cyan]: {avg1:.3f} Mboards/s")
    console.print(f"[magenta]{args.engine2}[/magenta]: {avg2:.3f} Mboards/s")
    if diff > 0:
        console.print(
            f"[bold]{args.engine1} faster by {diff:.3f} Mb/s (≈{pct:.2f} %).[/bold]"
        )
    else:
        console.print(
            f"[bold]{args.engine2} faster by {abs(diff):.3f} Mb/s (≈{pct:.2f} %).[/bold]"
        )

    if args.csv:
        with open(args.csv, "w", newline="") as fh:
            w = csv.writer(fh)
            w.writerow(["round", "engine", "mboards_per_sec"])
            w.writerows(csv_rows)
        console.print(f"[green]CSV written → {args.csv}[/green]")


if __name__ == "__main__":
    main()
