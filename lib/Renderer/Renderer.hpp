#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <config.h>
#include <Arduino.h>
#include <Esp.h>
#include "esp_task_wdt.h"
#include "esp_log.h"
#include <TFT_eSPI.h>
#include "rm67162.h"
#include "Latin_Hiragana_24.hpp"
#include "NotoSansMonoSCB20.hpp"
#include "NotoSansBold36.hpp"
#include "Final_Frontier_28.hpp"
#include <Assets.hpp>
#include <Data.hpp>
#include <Hardware.hpp>

// Reference points for text drawing
#define RP_TOPLEFT 0 // Top left (default)
#define RP_TOPCENTER 1 // Top centre
#define RP_TOPRIGHT 2 // Top right
#define RP_MIDDLELEFT 3 // Middle left
#define RP_CENTRELEFT 3 // Centre left, same as above
#define RP_MIDDLECENTER 4 // Middle centre
#define RP_CENTRECENTRE 4 // Centre centre, same as above
#define RP_MIDDLERIGHT 5 // Middle right
#define RP_CENTRERIGHT 5 // Centre right, same as above
#define RP_BOTTOMLEFT 6 // Bottom left
#define RP_BOTTOMCENTER 7 // Bottom centre
#define RP_BOTTOMRIGHT 8 // Bottom right
#define RP_LEFTBASELINE 9 // Left character baseline (Line the 'A' character would sit on)
#define RP_CENTREBASELINE 10 // Centre character baseline
#define RP_RIGHTBASELINE 11 // Right character baseline

// Personalization
#define DANGEROUS_RPM 90 // Percentage of redline to start flashing the RPM value

// Rev lights area dimensions
#define REVLIGHTS_HEIGHT 40 // Height of the rev lights area
#define REVLIGHTS_END 10 // Half of the rev lights area

// Declare TFT instances as extern
extern TFT_eSPI tft;
extern TFT_eSprite sprite;
extern uint8_t brightness[7];

namespace Renderer {
    /*******************************************************************
     * @brief Initialize the renderer
     * @param None
     * @since 1.0.0
     *******************************************************************/
    void init();

    /*******************************************************************
     * @brief Refresh the display: Draw all needed elements and push them to the sprite buffer
     * @param None No parameters needed
     * @since 1.0.0
     *******************************************************************/
    void refresh(void *pvParameters);

        /*******************************************************************
     * @brief Finalize the renderer and push the sprite buffer to the AMOLED display
     * @param None No parameters needed
     * @since 1.0.0
     *******************************************************************/
    void finalize();
}
namespace Draw {
    /*******************************************************************
     * @brief Draw the current selected gear
     * @param rp Reference point of the object
     * @param x X position of the object
     * @param y Y position of the object
     * @param size Size of the object
     * @param color Color of the object, in HEX format 5-6-5 bit format
     * @since 1.0.0
     *******************************************************************/
    void Gear(uint8_t rp = RP_TOPCENTER, uint16_t x = 268, uint8_t y = 45, uint8_t size = 3, uint16_t color = TFT_WHITE);

    /*******************************************************************
     * @brief Draw the suggested gear
     * @param rp Reference point of the object
     * @param x X position of the object
     * @param y Y position of the object
     * @param size Size of the object
     * @param color Color of the object, in HEX format 5-6-5 bit format
     * @since 1.0.0
     *******************************************************************/
    void SuggestedGear(uint8_t rp = RP_TOPCENTER, uint16_t x = 350, uint8_t y = 45, uint8_t size = 2, uint16_t color = TFT_WHITE);

    /*******************************************************************
     * @brief Draw a bar representing the rev lights
     * @param None Color, size and position are hardcoded to the top of the screen
     * @since 1.0.0
     *******************************************************************/
    void RevBar();

    /*******************************************************************
     * @brief Draw the current RPM value
     * @param rp Reference point of the object
     * @param x X position of the object
     * @param y Y position of the object
     * @param size Size of the object
     * @param color Color of the object, in HEX format 5-6-5 bit format
     * @since 1.0.0
     *******************************************************************/
    void RPM(uint8_t rp = RP_BOTTOMRIGHT, uint16_t x = 536, uint8_t y = 50, uint8_t size = 1, uint16_t color = TFT_WHITE);

    /*******************************************************************
     * @brief Draw the current speed value
     * @param rp Reference point of the object
     * @param x X position of the object
     * @param y Y position of the object
     * @param size Size of the object
     * @param color Color of the object, in HEX format 5-6-5 bit format
     * @since 1.0.0
     *******************************************************************/
    void Speed(uint8_t rp = RP_BOTTOMCENTER, uint16_t x = 268, uint8_t y = 240, uint8_t size = 1, uint16_t color = TFT_WHITE);

    /*******************************************************************
     * @brief Draw the ERS bar
     * @param None Color, size and position are hardcoded on the right side of the screen
     * @since 1.0.0
     *******************************************************************/
    void ERSbar();

    /*******************************************************************
     * @brief Draw the fuel bar
     * @param None Color, size and position are hardcoded on the right side of the screen
     * @since 1.0.0
     *******************************************************************/
    void FuelBar();

    /*******************************************************************
     * @brief Draw the throttle bar
     * @param None Color, size and position are hardcoded on the right side of the screen
     * @since 1.0.0
     *******************************************************************/
    void ThrottleBar();

    /*******************************************************************
     * @brief Draw the current lap data (lap time and lap number)
     * @param rp Reference point of the object
     * @param x X position of the object
     * @param y Y position of the object
     * @param size Size of the object
     * @param color Color of the object, in HEX format 5-6-5 bit format
     * @since 1.0.0
     *******************************************************************/
    void LapData(uint8_t rp = RP_BOTTOMRIGHT, uint16_t x = 536, uint8_t y = 240, uint8_t size = 1, uint16_t color = TFT_WHITE);

    void ERS(uint8_t rp = RP_TOPCENTER, uint16_t x = 350, uint8_t y = 134, uint8_t size = 1);

    void DRS(uint8_t rp = RP_TOPCENTER, uint16_t x = 350, uint8_t y = 154, uint8_t size = 1);

    void SafetyCar(uint8_t rp = RP_TOPCENTER, uint16_t x = 350, uint8_t y = 174, uint8_t size = 1);

} // namespace Draw
    /*******************************************************************
     * @brief Helper function: Convert RGB color to HEX color
     * @param red Red color component
     * @param green Green color component
     * @param blue Blue color component
     * @return HEX color value
     * @since 1.0.0
     *******************************************************************/
    uint16_t rgbToHex(uint8_t red, uint8_t green, uint8_t blue);

#endif // RENDERER_H