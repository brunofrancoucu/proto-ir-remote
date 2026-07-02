// Access point (Captive Portal)
#pragma once
#include <Arduino.h>
#include "utils.hpp"
#include "ir/emitter.hpp"
#include <DNSServer.h>
#include <map>
#include "ir/receiver.hpp"
#include "storage.hpp"
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Api Learn
extern bool g_is_learning;
std::map<String, std::map<String, SignalData>> databaseIR; // Signal data defined at storage.hpp
std::vector<uint16_t> lastCapturedSignal;
std::vector<uint16_t> lastSyncedSignal;

CycleComp<const char*, const char*> access_point(
    // Begin
    [](const char *ssid, const char* pass) {
        WiFi.softAP(ssid, pass);
        IPAddress IP = WiFi.softAPIP();
        dnsServer.start(DNS_PORT, "*", IP);

        // 1. Root page Path
        server.serveStatic("/style.css", LittleFS, "/style.css");
        server.serveStatic("/script.js", LittleFS, "/script.js");
        server.serveStatic("/", LittleFS, "/index.html");

        server.on("/api/learn/stop", HTTP_GET, []() {
            g_is_learning = false;    
            Serial.printf("Learn mode stop: %d\n", g_is_learning);
            server.send(200, "text/plain", "Stopped Listening");
        });
        
        server.on("/api/learn/start", HTTP_GET, []() {
            g_is_learning = true;
            lastCapturedSignal.clear(); // Empty the temporary buffer
            Serial.printf("Learn mode start: %d\n", g_is_learning);
            server.send(200, "text/plain", "Listening");
        });
        
        // Return new signal if the buffer has changed since last sync, otherwise return idle
        server.on("/api/learn/buffer", HTTP_GET, []() {
            if (lastCapturedSignal.size() > 2 && lastCapturedSignal != lastSyncedSignal) {
                lastSyncedSignal = lastCapturedSignal;
                String json = "{\"status\":\"success\", \"pulses\":" + String(lastCapturedSignal.size()) + "}";
                server.send(200, "application/json", json);
            } else {
                server.send(200, "application/json", "{\"status\":\"idle\"}");
            }
        });

        server.on("/api/learn/send", HTTP_GET, []() {
            if (server.hasArg("remote") && server.hasArg("btn")) {
                String remote = server.arg("remote");
                String btn = server.arg("btn");

                if (lastCapturedSignal.size() > 0) {
                    // Move the buffer into the permanent database
                    databaseIR[remote][btn].timings = lastCapturedSignal;
                    lastCapturedSignal.clear(); // Clear it for the next learning session
                    saveDatabase();
                    
                    Serial.printf("Assigned to %s -> %s\n", remote.c_str(), btn.c_str());
                    server.send(200, "text/plain", "Saved");
                } else {
                    server.send(400, "text/plain", "No signal in buffer to save!");
                }
            } else {
                server.send(400, "text/plain", "Missing params");
            }
        });

        // 2. Control route
        server.on("/api/transmit", HTTP_GET, []() {
            String remote = server.arg("remote");
            String btn = server.arg("btn");

            Serial.printf("Transmitting: %s -> %s\n", remote.c_str(), btn.c_str());

            // If btn exists
            if (databaseIR.count(remote) > 0 && databaseIR[remote].count(btn) > 0) {
                std::vector<uint16_t>& signal = databaseIR[remote][btn].timings;
                
                Serial.println("Found signal! Transmiting...");
                IrSender.sendRaw(signal.data(), signal.size(), 38); // Pure data vector
                
                server.send(200, "text/plain", "Sent");
            } else {
                Serial.println("No signal in control map.");
                server.send(404, "text/plain", "Not Found");
            }
        });

        server.onNotFound([]() {
            File file = LittleFS.open("/index.html", "r");
            if (!file) {
                server.send(404, "text/plain", "index.html not found");
                return;
            }
            server.streamFile(file, "text/html");
            file.close();
        });
        server.begin();
        Serial.println("Initialized web server at: http://" + IP.toString());
    },

    // Loop
    []() {
        dnsServer.processNextRequest();
        server.handleClient();
    }
);
