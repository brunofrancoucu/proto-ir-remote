// Access point (Captive Portal)
#pragma once
#include <Arduino.h>
#include "utils.hpp"
#include "ir/emitter.hpp"
#include <DNSServer.h>
// #include "hmi/ui.hpp"
#include <map>
#include "ir/receiver.hpp"

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);
const byte DNS_PORT = 53;
DNSServer dnsServer;


extern bool g_is_learning;
struct SignalData { std::vector<uint16_t> timings; };
// Dict: control name -> (btn name -> IR signal)
std::map<String, std::map<String, SignalData>> databaseIR;
String pendingRemote = "";
String pendingButton = "";
unsigned long learnStartTime = 0;
const unsigned long LEARN_TIMEOUT_MS = 10000; // 10 secs for copying

CycleComp<const char*, const char*> access_point(
    // Begin
    [](const char *ssid, const char* pass) { 
        LittleFS.begin(true); // unhandled exceptions
        Serial.println("Init Access Point...");
        WiFi.softAP(ssid, pass);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("Remote ip addr: ");
        Serial.println(IP); // Usually 192.168.4.1
        dnsServer.start(DNS_PORT, "*", IP);

        // 1. Root page Path
        server.serveStatic("/style.css", LittleFS, "/style.css");
        server.serveStatic("/script.js", LittleFS, "/script.js");
        server.serveStatic("/", LittleFS, "/index.html");

        server.on("/learn", HTTP_GET, []() {
            if (server.hasArg("remote") && server.hasArg("btn")) {
                pendingRemote = server.arg("remote");
                pendingButton = server.arg("btn");
                g_is_learning = true;
                learnStartTime = millis();
                
                IrReceiver.resume(); // Limpiar el buffer de lecturas viejas
                Serial.printf("Esperando señal IR para: %s -> %s\n", pendingRemote.c_str(), pendingButton.c_str());
                
                // Le respondemos rápido al navegador para no bloquear la web
                server.send(200, "text/plain", "Listening");
            } else {
                server.send(400, "text/plain", "Faltan parametros");
            }
        });

        // 2. Control route
        server.on("/ir", HTTP_GET, []() {
            String remote = server.arg("remote");
            String btn = server.arg("btn");
            
            Serial.printf("Asked to send: %s -> %s\n", remote.c_str(), btn.c_str());

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
            server.send(302, "text/plain", "");
        });
        
        server.begin();
        Serial.println("Web server initialized.");
    },

    // Loop
    []() {
        dnsServer.processNextRequest();
        server.handleClient();
    }
);
