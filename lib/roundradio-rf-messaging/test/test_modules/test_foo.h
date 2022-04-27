#pragma once
#include "foo/foo.h"
#include <unity.h>

void test_function_foo_foo(void) {
    Foo foo;
    TEST_ASSERT_TRUE(foo.foo());
}

void process_test_function_foo()
{
    UNITY_BEGIN();
    RUN_TEST(test_function_foo_foo);
    UNITY_END();
}
