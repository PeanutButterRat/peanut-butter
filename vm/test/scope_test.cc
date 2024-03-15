#include <gtest/gtest.h>

#include "../include/scope.h"

TEST(ScopeTests, BasicDefenitions) {
    auto scope = new Scope;
    scope->define("a", 5);
    scope->define("b", 6);

    ASSERT_EQ(scope->resolve("a"), 5);
    ASSERT_EQ(scope->resolve("b"), 6);
}

TEST(ScopeTests, ShadowingVariables) {
    auto scope = new Scope;
    scope->define("a", 5);
    scope->define("b", 10);
    scope = scope->push();
    scope->define("a", 20);

    ASSERT_EQ(scope->resolve("a"), 20);
    ASSERT_EQ(scope->resolve("b"), 10);

    scope = scope->pop();

    ASSERT_EQ(scope->resolve("a"), 5);
    ASSERT_EQ(scope->resolve("b"), 10);
}

TEST(ScopeTests, AlreadyDefinedVariable) {
    auto scope = new Scope;
    scope->define("a", 5);

    EXPECT_THROW(scope->define("a", 10), ScopeException);
}

TEST(ScopeTests, UndefinedVariable) {
    auto scope = new Scope;
    scope->define("a", 5);
    scope->resolve("a");

    EXPECT_THROW(scope->resolve("b"), ScopeException);
}

TEST(ScopeTests, DefinedAssigment) {
    auto scope = new Scope;
    scope->define("a", 5);
    scope->assign("a", 10);

    EXPECT_EQ(scope->resolve("a"), 10);
}

TEST(ScopeTests, UndefinedAssigment) {
    auto scope = new Scope;
    scope->define("a", 5);

    EXPECT_THROW(scope->assign("b", 10), ScopeException);
}