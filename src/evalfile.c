#include "evalfile.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/board.h"
#include "utils/fen.h"
#include "algorithm/bot.h"

// Returns NEGATIVE average centipawn loss.
//
// Assumption about dataset:
// - RHS scores are centipawn evaluations from WHITE'S perspective.
//   So:
//     higher  -> better for White
//     lower   -> better for Black
//
// For tuning, this file returns:
//   result = - average_cpl
//
// Therefore:
//   perfect choices everywhere ->  0.0
//   average CPL = 23.5         -> -23.5

#define EVALFILE_DEFAULT_NODES 5000
#define EVALFILE_MAX_PATH_LEN 1024
#define EVALFILE_MAX_LINE_LEN 65536
#define EVALFILE_MAX_MOVES 256
#define EVALFILE_MAX_UCI_LEN 8

// ---------- tiny helpers ----------

static char *ltrim(char *s)
{
    while (*s && isspace((unsigned char)*s))
        s++;
    return s;
}

static void rtrim_inplace(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1]))
        s[--n] = 0;
}

static char *trim(char *s)
{
    s = ltrim(s);
    rtrim_inplace(s);
    return s;
}

static int streqi(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return (*a == 0 && *b == 0);
}

// Parse:
//   evalfile "<path>" ...
//   evalfile C:\path\file.txt ...
static bool parse_path_token(const char *p_in, char *out, size_t out_sz, const char **after)
{
    const char *p = p_in;
    while (*p && isspace((unsigned char)*p))
        p++;

    if (!*p)
        return false;

    if (*p == '"')
    {
        p++;
        const char *q = strchr(p, '"');
        if (!q)
            return false;

        size_t n = (size_t)(q - p);
        if (n + 1 > out_sz)
            return false;

        memcpy(out, p, n);
        out[n] = 0;
        *after = q + 1;
        return true;
    }

    const char *q = p;
    while (*q && !isspace((unsigned char)*q))
        q++;

    size_t n = (size_t)(q - p);
    if (n + 1 > out_sz)
        return false;

    memcpy(out, p, n);
    out[n] = 0;
    *after = q;
    return true;
}

static void skip_ws(const char **p)
{
    while (**p && isspace((unsigned char)**p))
        (*p)++;
}

// Parse side to move directly from FEN.
// Returns true on success and sets *white_to_move.
// Returns false on malformed FEN.
static bool parse_white_to_move_from_fen(const char *fen, bool *white_to_move)
{
    const char *p = fen;
    if (!p)
        return false;

    // Skip board-part (field 1)
    while (*p && !isspace((unsigned char)*p))
        p++;

    while (*p && isspace((unsigned char)*p))
        p++;

    if (*p == 'w')
    {
        *white_to_move = true;
        return true;
    }

    if (*p == 'b')
    {
        *white_to_move = false;
        return true;
    }

    return false;
}

// Parses:
//   {"e2e4": 34, "g1f3": 19, ...}
// Returns number of parsed entries, or 0 on failure.
static int parse_move_score_map(const char *rhs,
                                char moves[][EVALFILE_MAX_UCI_LEN],
                                int32_t scores[],
                                int max_entries)
{
    const char *p = rhs;
    int n = 0;

    skip_ws(&p);
    if (*p != '{')
        return 0;
    p++;

    for (;;)
    {
        skip_ws(&p);

        if (*p == '}')
        {
            p++;
            return n;
        }

        if (n >= max_entries)
            return 0;

        if (*p != '"')
            return 0;
        p++;

        size_t k = 0;
        while (*p && *p != '"')
        {
            if (k + 1 >= EVALFILE_MAX_UCI_LEN)
                return 0;
            moves[n][k++] = *p++;
        }

        if (*p != '"')
            return 0;
        moves[n][k] = 0;
        p++;

        skip_ws(&p);
        if (*p != ':')
            return 0;
        p++;

        skip_ws(&p);

        char *endptr = NULL;
        long v = strtol(p, &endptr, 10);
        if (endptr == p)
            return 0;

        scores[n] = (int32_t)v;
        n++;
        p = endptr;

        skip_ws(&p);

        if (*p == ',')
        {
            p++;
            continue;
        }

        if (*p == '}')
        {
            p++;
            return n;
        }

        return 0;
    }
}

// Convert a WHITE-centric score to a SIDE-TO-MOVE-centric score.
// If White to move, keep as-is.
// If Black to move, negate so that larger is still better for the player to move.
static int32_t to_stm_score(int32_t white_score, bool white_to_move)
{
    return white_to_move ? white_score : -white_score;
}

// Computes negative centipawn loss for one position:
//
// Dataset scores are WHITE-centric.
// We convert to side-to-move scores before comparing moves.
//
// neg_cpl = -(best_stm_score - chosen_stm_score)
//
// Example:
//   White to move, best raw = +80, chosen raw = +30
//     => cpl = 50, neg_cpl = -50
//
//   Black to move, raw scores maybe:
//     best raw = -120, chosen raw = -40
//   Then stm scores become:
//     +120 and +40
//     => cpl = 80, neg_cpl = -80
static bool eval_position_neg_cpl(const char *fen_str, Board board, const char *rhs, double *out_neg_cpl)
{
    bool white_to_move = true;
    if (!parse_white_to_move_from_fen(fen_str, &white_to_move))
        return false;

    char moves[EVALFILE_MAX_MOVES][EVALFILE_MAX_UCI_LEN];
    int32_t raw_scores[EVALFILE_MAX_MOVES];

    int n = parse_move_score_map(rhs, moves, raw_scores, EVALFILE_MAX_MOVES);
    if (n <= 0)
        return false;

    BotResult bot_result = run_nodes_bot(board, EVALFILE_DEFAULT_NODES);
    const char *uci = bot_result.move;

    if (!uci || !*uci)
        return false;

    int best_idx = 0;
    int32_t best_stm_score = to_stm_score(raw_scores[0], white_to_move);

    int chosen_idx = -1;
    int32_t chosen_stm_score = 0;

    for (int i = 0; i < n; i++)
    {
        int32_t stm_score = to_stm_score(raw_scores[i], white_to_move);

        if (stm_score > best_stm_score)
        {
            best_stm_score = stm_score;
            best_idx = i;
        }

        if (streqi(moves[i], uci))
        {
            chosen_idx = i;
            chosen_stm_score = stm_score;
        }
    }

    (void)best_idx; // kept only for readability / debugging convenience

    if (chosen_idx < 0)
        return false;

    int32_t cpl = best_stm_score - chosen_stm_score;
    if (cpl < 0)
        cpl = 0;

    *out_neg_cpl = -(double)cpl;
    return true;
}

// ---------- public UCI command ----------

bool evalfile_run_uci_command(const char *line)
{
    if (!line)
        return false;

    const char *p = line;
    while (*p && isspace((unsigned char)*p))
        p++;

    if (strncmp(p, "evalfile", 8) != 0)
        return false;

    p += 8;

    char path[EVALFILE_MAX_PATH_LEN];
    const char *rest = NULL;
    if (!parse_path_token(p, path, sizeof(path), &rest))
    {
        printf("0.00000000000000000\n");
        fflush(stdout);
        return true;
    }

    int64_t limit = 0;

    // Optional:
    //   evalfile "file.txt" limit 1000
    char tmp[EVALFILE_MAX_PATH_LEN];
    strncpy(tmp, rest ? rest : "", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = 0;

    char *tok = strtok(tmp, " \t");
    while (tok)
    {
        if (streqi(tok, "limit"))
        {
            char *v = strtok(NULL, " \t");
            if (v)
                limit = (int64_t)atoll(v);
        }
        tok = strtok(NULL, " \t");
    }

    FILE *f = fopen(path, "r");
    if (!f)
    {
        printf("0.00000000000000000\n");
        fflush(stdout);
        return true;
    }

    char linebuf[EVALFILE_MAX_LINE_LEN];
    int64_t used = 0;
    double sum_neg_cpl = 0.0;

    while (fgets(linebuf, sizeof(linebuf), f))
    {
        if (limit > 0 && used >= limit)
            break;

        linebuf[strcspn(linebuf, "\r\n")] = 0;

        char *s = trim(linebuf);
        if (!*s || *s == '#')
            continue;

        char *bar = strchr(s, '|');
        if (!bar)
            continue;

        *bar = 0;
        char *fen_str = trim(s);
        char *rhs = trim(bar + 1);

        if (!*fen_str || !*rhs)
            continue;

        Board board = fen_to_board(fen_str);

        double neg_cpl = 0.0;
        if (!eval_position_neg_cpl(fen_str, board, rhs, &neg_cpl))
            continue;

        sum_neg_cpl += neg_cpl;
        used++;
    }

    fclose(f);

    if (used <= 0)
    {
        printf("0.00000000000000000\n");
        fflush(stdout);
        return true;
    }

    double result = sum_neg_cpl / (double)used;
    printf("%.17f\n", result);
    fflush(stdout);
    return true;
}