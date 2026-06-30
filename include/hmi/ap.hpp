// Access point (Captive Portal)
#pragma once
#include <Arduino.h>
#include "utils.hpp"
#include "ir/emitter.hpp"
#include <DNSServer.h>
#include "hmi/ui.hpp"

#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);
const byte DNS_PORT = 53;
DNSServer dnsServer;

CycleComp<const char*, const char*> access_point(
    // Begin
    [](const char *ssid, const char* pass) { 
        Serial.println("Init Access Point...");
        WiFi.softAP(ssid, pass);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("Remote ip addr: ");
        Serial.println(IP); // Usually 192.168.4.1
        dnsServer.start(DNS_PORT, "*", IP);

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

        server.onNotFound([]() {
            server.sendHeader("Location", "/", true);
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
