/*
 * F1 2022 Telemetry Display
 * Copyright (C) 2024 Just Koopen. All Rights Reserved.
 * 
 * AMOLED Racing Display on GitHub: https://github.com/jkoopen/amoled-race-display
 * My GitHub: https://github.com/jkoopen
 * 
 * Built and designed for the LILYGO® T-Display-S3 AMOLED
 * https://www.lilygo.cc/products/t-display-s3-amoled?variant=42837728526517
 */

#include <config.h> // Include the configuration file
TaskHandle_t refreshTask; // Create a task handle for the refresh task

void setup() {
  delay(1000);
  Hardware::init(921600); // Open and set the baud rate for the serial connection
  Renderer::init(); // Initialize the display and the renderer

  // Create a task to continuously refresh the display, framerate is capped to 60FPS.
  xTaskCreatePinnedToCore(Renderer::refresh, "refresh", 10000, NULL, 1, &refreshTask, 0);
}

void loop() {
  Decoder::loop(); // Loop through the decoder, decoding and parsing any data present
  delay(1); // Stability delay
}