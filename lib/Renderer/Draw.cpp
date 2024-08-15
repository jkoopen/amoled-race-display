#include "Renderer.h"

// Define TFT instances
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

void Draw::Gear(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    sprite.setTextColor(color); // Set the text color
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    if (playerData.gear[0] == 0)
    {
        sprite.unloadFont(); // Unload the font
        sprite.setTextSize(8);
        sprite.drawString("R", x, y + 40, 1); // Draw the text
    } else if (playerData.gear[0] == 1)
    {
        sprite.drawString("-", x, y, 7); // Draw the text
    } else
    {
         sprite.drawString(String(playerData.gear[0] - 1), x, y, 7); // Draw the text
    }
    sprite.unloadFont(); // Unload the font
}

void Draw::SuggestedGear(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    String gear; // Declare the gear variable
    if (playerData.gear[1] == 0)
    {
        gear = "-"; // No suggested gear
    } else {
        gear = String(playerData.gear[1]); // Set the gear to the suggested gear
    }
    sprite.setTextColor(color); // Set the text color
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    sprite.drawString(gear, x, y, 7); // Draw the text
}

void Draw::RevBar() {
    uint8_t revLights = playerData.revLightsPercent; // Set the rev lights value
    uint16_t color;

    // Determine the color based on the revLights value
    if (revLights >= 0 && revLights <= 18) {
        // Full green (0, 255, 0)
        color = TFT_ORANGE;
    } else if (revLights >= 19 && revLights <= 35) {
        // Full yellow (255, 255, 0)
        color = TFT_YELLOW;
    } else if (revLights >= 36 && revLights <= 49) {
        // Full red (255, 0, 0)
        color = TFT_GREEN;
    } else if (revLights >= 50 && revLights <= DANGEROUS_RPM) {
        // Full purple (128, 0, 128)
        color = TFT_PURPLE;
    } else {
        // Full blue (255, 0, 0)
        color = TFT_RED;
    }

    uint16_t revLightsWidth = map(revLights, 0, 100, 0, 536); // Map the rev lights value to the width range
    sprite.fillRect(0, 0, revLightsWidth, REVLIGHTS_HEIGHT, color); // Draw the rev lights area with the mapped width
    sprite.fillTriangle(0, REVLIGHTS_HEIGHT, 536, REVLIGHTS_END, 536, REVLIGHTS_HEIGHT, TFT_BLACK); // Draw the triangle to cover the remaining area
}

void Draw::RPM(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    if (playerData.revLightsPercent >= DANGEROUS_RPM) {
            sprite.setTextColor(TFT_RED); // Set the text color
    } else {
    sprite.setTextColor(color); // Set the text color
    }
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    sprite.loadFont(Latin_Hiragana_24); // Load the font
    sprite.drawString(String(playerData.rpm) + (String)" RPM", x, y); // Draw the text
    sprite.unloadFont(); // Unload the font
}

void Draw::Speed(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    if (playerData.gear[0] == 9 && playerData.revLightsPercent >= DANGEROUS_RPM) {
            sprite.setTextColor(TFT_RED); // Set the text color
    } else {
    sprite.setTextColor(color); // Set the text color
    }
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    sprite.drawString(String(playerData.speed), x, y, 7); // Draw the text
}

void Draw::ERSbar()
{
    sprite.pushImage(502, 50, 32, 32, (uint16_t*)Assets::ERS); // Push the ERS bar image to the sprite buffer
    sprite.drawRect(502, 90, 32, 110, TFT_WHITE); // Draw the ERS bar outline
}

void Draw::FuelBar()
{
    sprite.pushImage(450, 50, 32, 32, (uint16_t*)Assets::Fuel); // Push the fuel bar image to the sprite buffer
    sprite.drawRect(450, 90, 32, 110, TFT_WHITE); // Draw the fuel bar outline
}

void Draw::ThrottleBar()
{
    sprite.drawRect(398, 90, 32, 110, TFT_WHITE); // Draw the throttle bar outline (always)

    if (playerData.paddles[1] > 0) {
        sprite.pushImage(398, 50, 32, 32, (uint16_t*)Assets::Brake); // Push the brake bar image to the sprite buffer
        uint8_t brake = map(playerData.paddles[1], 0, 100, 0, 105); // Map the brake value to the height range

        sprite.fillRect(400, 197 - brake, 28, brake, TFT_RED); // Draw the brake bar with the mapped height
    } else {
        sprite.pushImage(398, 50, 32, 32, (uint16_t*)Assets::Throttle); // Push the throttle bar image to the sprite buffer
        uint8_t throttle = map(playerData.paddles[0], 0, 100, 0, 105); // Map the throttle value to the height range

        if (playerData.paddles[0] > 99) {
            sprite.fillRect(400, 197 - throttle, 28, throttle, TFT_BLUE); // Draw the throttle blue indicating full throttle
        } else {
            sprite.fillRect(400, 197 - throttle, 28, throttle, TFT_GREEN); // Draw the throttle bar with the mapped height
        }
    }
}

void Draw::LapData(uint8_t rp, uint16_t x, uint8_t y, uint8_t size, uint16_t color)
{
    char lapData[50]; // Buffer to hold the formatted string
    sprintf(lapData, "L:%d/%d   %d:%02d.%03d", 
            playerData.lapData[0], 
            playerData.lapData[1], 
            playerData.lapTime[0], 
            playerData.lapTime[1], 
            playerData.lapTime[2]); // Format the lap time string

    sprite.setTextColor(color); // Set the text color
    sprite.setTextSize(size); // Set the text size
    sprite.setTextDatum(rp); // Set the reference point of the text
    sprite.loadFont(Latin_Hiragana_24); // Load the font
    sprite.drawString(lapData, x, y); // Draw the text
    sprite.unloadFont(); // Unload the font
}

///---------------------------------------------------------------------------------------------------------------------

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