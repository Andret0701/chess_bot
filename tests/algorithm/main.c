#include "unity.h"

#include "algorithm/zobrist_hash.h"

#include "test_threefold_repetition.h"
#include "heuristic/test_symetric_heuristic.h"

void setUp(void)
{
}
void tearDown(void) {}

int main(void)
{
    init_zobrist_keys();

    UNITY_BEGIN();
    RUN_TEST(test_symmetry_wrapper);
    RUN_TEST(test_threefold_repetition_wrapper);
    return UNITY_END();
}
