#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"
#include "ir/emitter.hpp"
#include "ir/receiver.hpp"

void setup() {
    Serial.begin(9600);
    // Digital Pin Out
    receiver.setup(15); 
    emitter.setup(13);
}

void loop() {
    receiver.run(0);    
    emitter.run(5000);  // Emit every 5s
}