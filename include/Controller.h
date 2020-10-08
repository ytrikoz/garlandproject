#pragma once

#include <Arduino.h>

#include "consts.h"
#include "utils.h"

#include "Pin.h"

class Controller {
    Pin _pins[NUM_PINS] = {15, 16, 17, 5, 18, 19, 21, 23};
    uint8_t _pos;
    uint16_t _cycleDuration;
    unsigned long _cycleStarted;
    unsigned long _lastRead;

   public:
    Controller() : _pos{0},
                   _cycleDuration{0},
                   _cycleStarted{0},
                   _lastRead{0} {
        analogReadResolution(12);
        adcAttachPin(CONTROL_PIN);
    }

    float getThorottlePosition() {
        uint16_t raw = analogRead(CONTROL_PIN);
        uint16_t norm = constrain(raw, MIN_POS, MAX_POS);
        // TODO Filter
        float res = 0;
        if (norm > DEAD_ZONE) {
            res = (float)map(norm, DEAD_ZONE, MAX_POS, 0, 1000) / 1000.0f;
        }
        Serial.printf("raw:%d,norm:%d,res:%f\r\n", raw, norm, res);
        return res;
    }

    uint16_t getCycleInterval(float throttle) {
        return map(throttle * 1000, 0, 1000, NO_THROTTLE_INTERVAL, FULL_THROTTLE_INTERVAL);
    }

    void setCycleDuration(uint16_t value) {
        Serial.printf("cycle:%d\r\n", value);
        _cycleDuration = value;
        next();
    }

    bool isCycleDone() {
        return (_cycleDuration > 0) && (millis_since(_cycleStarted) > _cycleDuration);
    }

    void next() {
        _pins[_pos].off();

        if (_cycleDuration > 0) {
            if (++_pos >= NUM_PINS) _pos = 0;
            _pins[_pos].on();
            _cycleStarted = millis();
        }
    }

    void loop() {
        if (millis_since(_lastRead) > FULL_THROTTLE_INTERVAL * 10) {
            _lastRead = millis();
            float pos = getThorottlePosition();
            uint16_t cycle = getCycleInterval(pos);

            if (_cycleDuration != cycle) {
                setCycleDuration(cycle);
            }

            if (isCycleDone()) {
                next();
            }
        }
    }
};