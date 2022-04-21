#pragma once
#include "calculator/calculator.h"
#include <unity.h>

void test_function_calculator_addition(void) {
    Calculator calc;
    TEST_ASSERT_EQUAL(32, calc.add(25, 7));
}

void test_function_calculator_subtraction(void) {
    Calculator calc;
    TEST_ASSERT_EQUAL(20, calc.sub(23, 3));
}

void test_function_calculator_multiplication(void) {
    Calculator calc;
    TEST_ASSERT_EQUAL(50, calc.mul(25, 2));
}

void test_function_calculator_division(void) {
    Calculator calc;
    TEST_ASSERT_EQUAL(32, calc.div(96, 3));
}

void process_test_function_calculator()
{
    UNITY_BEGIN();
    RUN_TEST(test_function_calculator_addition);
    RUN_TEST(test_function_calculator_subtraction);
    RUN_TEST(test_function_calculator_multiplication);
    RUN_TEST(test_function_calculator_division);
    UNITY_END();
}
