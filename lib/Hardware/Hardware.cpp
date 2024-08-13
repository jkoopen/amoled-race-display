#include "Hardware.h"

uint8_t setBrightness = 3; // The set brightness level
bool pressed = false; // Button press flag

void Hardware::init(uint32_t baud) {
    pinMode(PIN_BUTTON_1, INPUT);
    pinMode(PIN_BUTTON_2, INPUT);
    pinMode(PIN_LED, OUTPUT);
    Serial.begin(baud);
    Serial.setRxBufferSize(1024);
}

void Hardware::cycleBrightness() {
        if (digitalRead(PIN_BUTTON_2) == 0) {
        if (!pressed) {
            pressed = true;
            setBrightness++;
            if (setBrightness == 7) setBrightness = 0;
            lcd_brightness(brightness[setBrightness]); // Adjust LCD brightness
        }
    } else {
        pressed = false;
    }
}