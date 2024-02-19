#include <gtest/gtest.h>

TEST(ScannerTest, BasicAssertions) {
    EXPECT_STRNE("not", "equal");
    EXPECT_EQ(1 + 1, 2);
}
