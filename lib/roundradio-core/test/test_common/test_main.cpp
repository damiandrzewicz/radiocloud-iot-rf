#include <unity.h>
#include "../test_modules/test_calculator.h"
#include "../test_modules/test_foo.h"

void process() {
    UNITY_BEGIN();
    process_test_function_foo();
    process_test_function_calculator();
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif