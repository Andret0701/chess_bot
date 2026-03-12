#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    EVALFILE_MODE_TOP1 = 0,   // top-1 accuracy
    EVALFILE_MODE_POLICY = 1, // mean softmax p(label)
    EVALFILE_MODE_LOGP = 2    // exp(mean log p(label))  (geometric mean)
} EvalfileMode;

typedef struct
{
    int64_t positions_total;   // lines processed (including skipped blanks/comments)
    int64_t positions_used;    // valid parsed positions used in metric
    int64_t positions_skipped; // malformed/unsupported lines
    int64_t label_missing;     // label move not found among legal moves
    int64_t top1_correct;      // number of positions where best move == label

    double top1_acc; // top1_correct / positions_used

    double avg_p;  // mean p(label) (softmax)
    double geom_p; // exp(mean log p(label))

    double tau; // temperature used
    EvalfileMode mode;
} EvalfileResult;

// Core evaluator (no printing)
bool evalfile_evaluate_path(const char *path, double tau, EvalfileMode mode, int64_t limit, EvalfileResult *out);

// Convenience: parses and runs "evalfile ..." command and PRINTS results.
// Returns true iff line begins with "evalfile".
bool evalfile_run_uci_command(const char *line);

const char *evalfile_mode_to_string(EvalfileMode mode);
