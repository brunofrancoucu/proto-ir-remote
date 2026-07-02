// Emitter
#pragma once
#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"

CycleComp<int> emitter(
    // Begin
    [](int PIN) { 
        IrSender.begin(PIN);
    },
    // Loop
    []() {} // IrSender.sendRaw(tvDownRaw, sizeof(tvDownRaw) / sizeof(tvDownRaw[0]), 38);
);