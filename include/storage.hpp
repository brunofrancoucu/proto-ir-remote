#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <vector> // This was missing!
#include <map>

// 1. Define the struct so the file knows what it is
struct SignalData { std::vector<uint16_t> timings; };
extern std::map<String, std::map<String, SignalData>> databaseIR;


CycleComp<> storage(
    // Begin
    []() {
        LittleFS.begin(true); // unhandled exceptions

        // Load the JSON file into your map
        if (!LittleFS.exists("/database.json")) {
            Serial.println("No database found. Starting fresh.");
            return; 
        }

        File file = LittleFS.open("/database.json", "r");
        JsonDocument doc; 
        deserializeJson(doc, file);
        file.close();

        for (JsonPair remote : doc.as<JsonObject>()) {
            String rName = remote.key().c_str();
            for (JsonPair btn : remote.value().as<JsonObject>()) {
                String bName = btn.key().c_str();
                JsonArray arr = btn.value().as<JsonArray>();
                for (uint16_t t : arr) {
                    databaseIR[rName][bName].timings.push_back(t);
                }
            }
        }
    },

    // Loop
    []() {}
);


// Save the entire map to a JSON file
inline void saveDatabase() {
    JsonDocument doc; 

    for (auto const& remotePair : databaseIR) {
        String remoteName = remotePair.first;
        for (auto const& btnPair : remotePair.second) {
            String btnName = btnPair.first;
            
            // ArduinoJson v7 standard syntax
            JsonArray arr = doc[remoteName][btnName].to<JsonArray>();
            
            for (uint16_t t : btnPair.second.timings) {
                arr.add(t);
            }
        }
    }

    File file = LittleFS.open("/database.json", "w");
    serializeJson(doc, file); 
    file.close();
}