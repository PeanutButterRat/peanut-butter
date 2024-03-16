#include <gtest/gtest.h>

#include "../include/value.h"

TEST(ValueTests, Equality) {
    Value three{3};
    Value five{5};
    Value hello{"Hello"};
    Value world{"World"};
    Value truthy{true};
    Value falsey{false};

    ASSERT_EQ(three, three);
    ASSERT_EQ(hello, hello);
    ASSERT_EQ(truthy, truthy);
    ASSERT_EQ(falsey, falsey);

    ASSERT_NE(three, five);
    ASSERT_NE(hello, world);
    ASSERT_NE(hello, falsey);
    ASSERT_NE(falsey, three);
    ASSERT_NE(three, hello);
}

TEST(ValueTests, TruthyFalsey) {
    Value five{5};
    Value zero{0};
    Value negative{-1};
    Value hello{"Hello"};
    Value empty{""};
    Value truthy{true};
    Value falsey{false};

    ASSERT_TRUE(five.truthy());
    ASSERT_TRUE(negative.truthy());
    ASSERT_TRUE(hello.truthy());
    ASSERT_TRUE(truthy.truthy());

    ASSERT_TRUE(zero.falsey());
    ASSERT_TRUE(empty.falsey());
    ASSERT_TRUE(falsey.falsey());
}

TEST(ValueTests, Comparisons) {
    Value negative{-1};
    Value zero{0};
    Value three{3};
    Value eight{8};
    Value abc{"ABC"};
    Value def{"DEF"};

    ASSERT_LT(negative, zero);
    ASSERT_LT(zero, three);
    ASSERT_LT(three, eight);
    ASSERT_GT(zero, negative);
    ASSERT_GT(three, zero);
    ASSERT_GT(eight, three);

    ASSERT_LT(abc, def);
    ASSERT_GT(def, abc);
}

TEST(ValueTests, OperationExceptions) {
    Value three{3};
    Value abc{"ABC"};
    Value truthy{true};

    EXPECT_THROW(three > abc, InvalidOperationException);
    EXPECT_THROW(three < abc, InvalidOperationException);
    EXPECT_THROW(abc > truthy, InvalidOperationException);
    EXPECT_THROW(abc < truthy, InvalidOperationException);
    EXPECT_THROW(truthy > three, InvalidOperationException);
    EXPECT_THROW(truthy < three, InvalidOperationException);

    EXPECT_THROW(three * abc, InvalidOperationException);
    EXPECT_THROW(three / abc, InvalidOperationException);
    EXPECT_THROW(three % abc, InvalidOperationException);
    EXPECT_THROW(abc * truthy, InvalidOperationException);
    EXPECT_THROW(abc / truthy, InvalidOperationException);
    EXPECT_THROW(abc % truthy, InvalidOperationException);
    EXPECT_THROW(truthy * three, InvalidOperationException);
    EXPECT_THROW(truthy / three, InvalidOperationException);
    EXPECT_THROW(truthy % three, InvalidOperationException);

    EXPECT_THROW(three + abc, InvalidOperationException);
    EXPECT_THROW(three - abc, InvalidOperationException);
    EXPECT_THROW(abc + truthy, InvalidOperationException);
    EXPECT_THROW(abc - truthy, InvalidOperationException);
    EXPECT_THROW(truthy + three, InvalidOperationException);
    EXPECT_THROW(truthy - three, InvalidOperationException);
}

TEST(ValueTests, Casting) {
    Value three{3};
    Value abc{"ABC"};
    Value truthy{true};

    EXPECT_EQ(three.integer(), 3);
    EXPECT_EQ(abc.string(), "ABC");
    EXPECT_EQ(truthy.boolean(), true);

    EXPECT_THROW(std::ignore = three.string(), BadCastException);
    EXPECT_THROW(std::ignore = three.boolean(), BadCastException);
    EXPECT_THROW(std::ignore = abc.integer(), BadCastException);
    EXPECT_THROW(std::ignore = abc.boolean(), BadCastException);
    EXPECT_THROW(std::ignore = truthy.integer(), BadCastException);
    EXPECT_THROW(std::ignore = truthy.string(), BadCastException);
}