#pragma once
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "hmi/ap.hpp"


// struct SignalData { std::vector<uint16_t> timings; };
// Dict: control name -> (btn name -> IR signal)
extern std::map<String, std::map<String, SignalData>> databaseIR;

// Save the entire map to a JSON file
void saveDatabase() {
    JsonDocument doc; // Updated to v7 standard
    // DynamicJsonDocument doc(16384); // Size based on your expected signal count

    for (auto const& [remoteName, buttons] : databaseIR) {
        for (auto const& [btnName, data] : buttons) {
            JsonArray arr = doc[remoteName].createNestedArray(btnName);
            for (uint16_t t : data.timings) arr.add(t);
        }
    }

    File file = LittleFS.open("/database.json", "w");
    serializeJson(doc, file); // This automatically handles sizing
    file.close();
}

// Load the JSON file into your map
void loadDatabase() {
    if (!LittleFS.exists("/database.json")) return;

    File file = LittleFS.open("/database.json", "r");
    JsonDocument doc; // Updated type
    deserializeJson(doc, file);
    file.close();

    for (JsonPair remote : doc.as<JsonObject>()) {
        String rName = remote.key().c_str();
        for (JsonPair btn : remote.value().as<JsonObject>()) {
            String bName = btn.key().c_str();
            JsonArray arr = btn.value().as<JsonArray>();
            for (uint16_t t : arr) databaseIR[rName][bName].timings.push_back(t);
        }
    }
}