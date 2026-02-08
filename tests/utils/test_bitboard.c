#include "unity.h"
#include "utils/bitboard.h"

void setUp(void) {}
void tearDown(void) {}

void test_A1_mask_is_nonzero(void)
{
    TEST_ASSERT_NOT_EQUAL_UINT64_MESSAGE(1ULL, A1_MASK, "A1_MASK should not be zero");
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_A1_mask_is_nonzero);
    return UNITY_END();
}
