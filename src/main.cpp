#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"
#include "ir/emitter.hpp"
#include "ir/receiver.hpp"
#include "hmi/ap.hpp"

// Temp
#include "storage.hpp"

void setup() {
    Serial.begin(9600);
    // Digital Pin Out
    receiver.setup(15); 
    emitter.setup(13);
    access_point.setup("IR_Control", "12345678");
    loadDatabase(); // after fs begin
}

void loop() {
    receiver.run();
    emitter.run(5000);  // Emit every 5s
    access_point.run();
}