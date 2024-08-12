#include "Renderer.h"

// Define TFT instances
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

uint8_t brightness[7] = {100, 120, 140, 180, 200, 230, 254}; // Brightness of the AMOLED display

void Renderer::init()
{
    sprite.createSprite(536, 240); // Create sprite with dimensions
    sprite.setSwapBytes(true);

    rm67162_init(); // Initialize AMOLED LCD
    lcd_setRotation(1); // Set rotation to 90 degrees

    lcd_brightness(brightness[6]); // Set initial brightness (index 6 is the last element)

    // Check if the display is awake
    uint8_t displayStatus = tft.readcommand8(0x0A); // Read display status

    // If display status is 0, try to wake up the display
    if (displayStatus == 0) {
        lcd_display_on();
        delay(500); // Increase delay to give more time for the display to wake up
    }
}

void Renderer::drawGear(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    sprite.setTextColor(color); // Set the text color
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    sprite.drawString(String(playerData.gear[0]), x, y, 7); // Draw the text
}

void Renderer::drawSuggestedGear(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    sprite.setTextColor(color); // Set the text color
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    sprite.drawString(String(playerData.gear[1]), x, y, 7); // Draw the text
}

#define REVLIGHTS_HEIGHT 40 // Height of the rev lights area
#define REVLIGHTS_HALF 10 // Half of the rev lights area

void Renderer::drawRevLights() {
    uint8_t revLights = playerData.revLightsPercent; // Set the rev lights value
    uint16_t color;

    // Determine the color based on the revLights value
    if (revLights >= 0 && revLights <= 24) {
        // Full green (0, 255, 0)
        color = TFT_RED;
    } else if (revLights >= 25 && revLights <= 49) {
        // Full red (255, 0, 0)
        color = TFT_GREEN;
    } else if (revLights >= 50 && revLights <= 84) {
        // Full purple (128, 0, 128)
        color = TFT_PURPLE;
    } else {
        // Full blue (255, 0, 0)
        color = TFT_RED;
    }

    uint16_t revLightsWidth = map(revLights, 0, 100, 0, 536); // Map the rev lights value to the width range
    sprite.fillRect(0, 0, revLightsWidth, REVLIGHTS_HEIGHT, color); // Draw the rev lights area with the mapped width
    sprite.fillTriangle(0, REVLIGHTS_HEIGHT, 536, REVLIGHTS_HALF, 536, REVLIGHTS_HEIGHT, TFT_BLACK); // Draw the triangle to cover the remaining area
}

void Renderer::drawRPM(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    sprite.setTextColor(color); // Set the text color
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    sprite.loadFont(Latin_Hiragana_24); // Load the font
    sprite.drawString(String(playerData.rpm) + (String)" RPM", x, y); // Draw the text
    sprite.unloadFont(); // Unload the font
}

void Renderer::finalize()
{
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer()); // Push the sprite buffer to LCD
    sprite.fillSprite(TFT_BLACK); // Clear the sprite with black color
}

uint16_t rgbToHex(uint8_t red, uint8_t green, uint8_t blue)
{
    // Ensure the color components are within the valid range
    red = red & 0xFF;
    green = green & 0xFF;
    blue = blue & 0xFF;

    // Convert 8-bit components to 5-6-5 bit format
    uint16_t r = (red >> 3) & 0x1F;   // 5 bits for red
    uint16_t g = (green >> 2) & 0x3F; // 6 bits for green
    uint16_t b = (blue >> 3) & 0x1F;  // 5 bits for blue

    // Combine the components into a single 16-bit value
    uint16_t hexValue = (r << 11) | (g << 5) | b;

    return hexValue;
}