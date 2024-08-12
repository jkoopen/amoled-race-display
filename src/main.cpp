#include <config.h>

// Create TFT instances

void setup() {
    pinMode(PIN_BUTTON_2, INPUT_PULLUP);
    pinMode(PIN_BUTTON_1, INPUT_PULLUP);
    pinMode(led, OUTPUT);

    playerData.gear[0] = 5;
    playerData.gear[1] = 5;
    playerData.revLightsPercent = 0;
    playerData.rpm = 10000;
    Renderer::init();
}

void loop() {
    static int step = 2;
    static int direction = 1;

    // Update revLightsPercent
    playerData.revLightsPercent += step * direction;

    // Reverse direction if limits are reached
    if (playerData.revLightsPercent >= 100 || playerData.revLightsPercent <= 0) {
        direction *= -1;
    }

    // Draw the rev lights with the updated percentage
    Renderer::drawRevLights();
    Renderer::drawGear();
    Renderer::drawSuggestedGear();
    Renderer::drawRPM();
    Renderer::finalize();

    // Delay to make the changes visible
    delay(20);
}