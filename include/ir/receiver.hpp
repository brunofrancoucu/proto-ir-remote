// Receiver
#pragma once
#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"

CycleComp<int> receiver(
    // Begin
    [](int PIN) { 
        IrReceiver.begin(PIN, ENABLE_LED_FEEDBACK); 
        Serial.println("Receiver active.");
    },
    // Loop
    []() {
        if (IrReceiver.decode()) {
            Serial.println("\n----------------------------------------");
            Serial.print("Detected protocol: "); // 0: unknown 
            Serial.println(IrReceiver.decodedIRData.protocol);
            
            // Print array C++
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            Serial.println("----------------------------------------");

            Serial.println("-> Signal picked up by receiver!");
            
            IrReceiver.resume(); 
        }
    }
);
