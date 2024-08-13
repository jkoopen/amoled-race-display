#include <config.h>

TaskHandle_t refreshTask;

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  Hardware::init(); // Open and set the baud rate for the serial connection
  Renderer::init(); // Initialize the display and the renderer

  // Create a task to continuously refresh the display, framerate is capped to 60FPS.
  xTaskCreatePinnedToCore(Renderer::refresh, "refresh", 10000, NULL, 1, &refreshTask, 0);
}

void loop() {
  Decoder::loop(); // Loop through the decoder, decoding and parsing any data present
  Hardware::cycleBrightness(); // Cycle the brightness of the display when the button is pressed
  delay(1); // Stability delay
}