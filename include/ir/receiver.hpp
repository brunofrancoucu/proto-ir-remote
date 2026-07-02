// Receiver
#pragma once
#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"
#include "ap.hpp"

// Temp
#include "storage.hpp"

bool g_is_learning = false;

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
                // 1. Default
                Serial.println("\n----------------------------------------");
                Serial.print("Detected protocol: "); // 0: unknown 
                Serial.println(IrReceiver.decodedIRData.protocol);
                
                // Print array C++
                IrReceiver.printIRResultRawFormatted(&Serial, true);
                Serial.println("----------------------------------------");
    
                Serial.println("-> Signal picked up by receiver!");

                // 2. Store
                Serial.println("Captured learning signal!");
            
                std::vector<uint16_t> newSignal; // Dynamic buffer vector storage (skip complex pointer)

                // Skip index 0 (initial gap/noise)
                for (unsigned int i = 1; i < IrReceiver.irparams.rawlen; i++) {
                    uint16_t time_us = IrReceiver.irparams.rawbuf[i] * MICROS_PER_TICK;
                    newSignal.push_back(time_us);
                }

                databaseIR[pendingRemote][pendingButton].timings = newSignal; // Store at dictionary
                
                Serial.printf("Signal stored in RAM, signal size: %d\n", newSignal.size());

                g_is_learning = false; // Apagamos el modo
                saveDatabase();
                IrReceiver.resume();
            }
        }
    }
);
