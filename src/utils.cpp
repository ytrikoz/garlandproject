#include "utils.h"

#include <Arduino.h>

unsigned long millis_since(unsigned long sinse) {
    return millis_passed(sinse, millis());
}

unsigned long millis_passed(unsigned long start, unsigned long finish) {
    unsigned long result = 0;
    if (start <= finish) {
        unsigned long passed = finish - start;
        if (passed <= LONG_MAX) {
            result = static_cast<long>(passed);
        } else {
            result = static_cast<long>((LONG_MAX - finish) + start + 1u);
        }
    } else {
        unsigned long passed = start - finish;
        if (passed <= LONG_MAX) {
            result = static_cast<long>(passed);
            result = -1 * result;
        } else {
            result = static_cast<long>((LONG_MAX - start) + finish + 1u);
            result = -1 * result;
        }
    }
    return result;
}
