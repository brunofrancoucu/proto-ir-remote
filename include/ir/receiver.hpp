// Receiver
#pragma once
#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"
#include "ap.hpp"

bool g_is_learning = false;
extern std::vector<uint16_t> lastCapturedSignal;

CycleComp<int> receiver(
    // Begin
    [](int PIN) { 
        IrReceiver.begin(PIN, ENABLE_LED_FEEDBACK); 
        Serial.println("Receiver active.");
    },
    // Loop
    []() {
        if (g_is_learning) {
            if (IrReceiver.decode()) {
                std::vector<uint16_t> newSignal; // Dynamic buffer vector storage (skip complex pointer)
    
                // Skip index 0 (initial gap/noise)
                for (unsigned int i = 1; i < IrReceiver.irparams.rawlen; i++) {
                    uint16_t time_us = IrReceiver.irparams.rawbuf[i] * MICROS_PER_TICK;
                    newSignal.push_back(time_us);
                }

                // Skip noise
                if (newSignal.size() > 2) {
                    // 1. Default
                    Serial.println("\n----------------------------------------");
                    Serial.print("Detected protocol: "); // 0: unknown 
                    Serial.println(IrReceiver.decodedIRData.protocol);
                    
                    // Print array C++
                    IrReceiver.printIRResultRawFormatted(&Serial, true);
                    Serial.println("----------------------------------------");
        
                    Serial.println("-> Signal picked up by receiver!");
    
                    // 2. Store
                    
                    lastCapturedSignal = newSignal; // Save to tmp buffer
                    Serial.printf("Signal stored in RAM buffer, signal size: %d\n", newSignal.size());
                }

                IrReceiver.resume();
            }
        }
    }
);
