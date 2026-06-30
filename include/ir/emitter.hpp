// Emitter
#pragma once
#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"

// The raw timing array you captured earlier
uint16_t tvPowerRaw[67] = {
    4600, 4400, 
    650, 1650, 650, 1600, 650, 1650, 600, 500, 
    650, 500, 600, 550, 600, 550, 600, 550, 
    600, 1650, 600, 1700, 550, 1750, 550, 550, 
    600, 550, 550, 600, 550, 550, 600, 550, 
    600, 550, 600, 1700, 550, 1700, 600, 550, 
    600, 550, 550, 1700, 600, 1700, 550, 1700, 
    600, 1650, 600, 550, 600, 550, 600, 1700, 
    550, 1700, 600, 550, 550, 600, 550, 600, 
    600
};

uint16_t airConUp[131] = {
    // Encabezado
    3450, 1800,
    
    // Trama de datos estandarizada
    400, 450,  400, 1350, 400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 1350, 400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 1350, 400, 1350, 400, 1350,
    400, 450,  400, 450,  400, 1350, 400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    400, 450,  400, 1350, 400, 1350, 400, 450,
    400, 450,  400, 450,  400, 450,  400, 450,
    
    // Bit de parada final
    400
};

const uint16_t tvDownRaw[25] = {
    // Encabezado
    1350, 1150,
    
    // Trama de datos (Marcas constantes a ~450µs, espacios variables)
    450, 550, 
    450, 850, 
    450, 2450, 
    450, 550,
    450, 1450, 
    450, 2050, 
    450, 1750, 
    450, 850,
    450, 1750, 
    450, 2650, 
    450, 1750,
    
    // Bit de parada
    450
};

CycleComp<int> emitter(
    // Behin
    [](int PIN) { 
        IrSender.begin(PIN); 
        Serial.println("Emitter active.");
    },
    // Loop
    []() {
        Serial.println("<- Transmitting raw array...");
        IrSender.sendRaw(tvDownRaw, sizeof(tvDownRaw) / sizeof(tvDownRaw[0]), 38);
        // Ráfaga de 3 envíos para asegurar el 'hit' en el receptor del TV
        // for(int i = 0; i < 3; i++) {
        //     IrSender.sendRaw(tvDownRaw, sizeof(tvDownRaw) / sizeof(tvDownRaw[0]), 38);
        //     delay(40); // Gap de separación estándar
        // }

    }
);