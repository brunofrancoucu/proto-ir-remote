// Access point

#include <Arduino.h>
#include "utils.hpp"
#include "ir/emitter.hpp"
#include "hmi/ui.hpp"

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Control_ESP32";
const char* password = "password123"; // At least 8 chars

WebServer server(80);


CycleComp<int> receiver(
    // Begin
    [](int PIN) { 
        Serial.println("Init Access Point...");
        WiFi.softAP(ssid, password);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("Remote ip addr: ");
        Serial.println(IP); // Usually 192.168.4.1

        // 1. Root page Path
        server.on("/", HTTP_GET, []() {
            server.send(200, "text/html", index_html);
        });

        // 2. Control route
        server.on("/ir", HTTP_GET, []() {
            if (server.hasArg("btn")) {
                String btnPressed = server.arg("btn");
                Serial.print("Comando recibido: ");
                Serial.println(btnPressed);

                if (btnPressed == "power") {
                    IrSender.sendRaw(tvPowerRaw, sizeof(tvPowerRaw) / sizeof(tvPowerRaw[0]), 38);
                } 
                else if (btnPressed == "down") {
                    IrSender.sendRaw(tvDownRaw, sizeof(tvDownRaw) / sizeof(tvDownRaw[0]), 38);
                }

                server.send(200, "text/plain", "OK");
            } else {
                server.send(400, "text/plain", "btn param missing");
            }
        });

        server.begin();
        Serial.println("Web server initialized.");
    },

    // Loop
    []() {
        server.handleClient();
    }
);
