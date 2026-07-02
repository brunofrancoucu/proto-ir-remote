#include <Arduino.h>
#include <IRremote.hpp>
#include "utils.hpp"
#include "ir/emitter.hpp"
#include "ir/receiver.hpp"
#include "ap.hpp"
#include "storage.hpp"

void setup() {
    Serial.begin(9600);
    storage.setup();
    // Digital Pin Out
    receiver.setup(15); 
    emitter.setup(13);
    access_point.setup("IR_Control", "12345678");
}

void loop() {
    receiver.run();
    access_point.run();
}