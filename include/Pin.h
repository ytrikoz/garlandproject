#pragma once

#include <Arduino.h>

#include "consts.h"

struct Pin {
   private:
    uint8_t _num;
    bool _state;

   public:
    Pin(uint8_t num) : _num{num},
                       _state{STATE_OFF} {
        pinMode(_num, OUTPUT);
        applyState();
    };

    void on() {
        _state = STATE_ON;
        applyState();
    }

    void off() {
        _state = STATE_OFF;
        applyState();
    }

    void toggle() {
        _state = !_state;
        applyState();
    };

    void applyState() {
        digitalWrite(_num, _state);
    }
};
