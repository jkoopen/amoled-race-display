#include "Renderer.h"

void Renderer::refresh(void *pvParameters)
{
    esp_task_wdt_add(NULL); // Add the task to the watchdog timer
    while (true) // Loop forever
    {
        esp_task_wdt_reset(); // Reset the watchdog timer at the beginning of the loop
        vTaskPrioritySet(NULL, 1); // Set the priority of the task to 1
        uint64_t startMillis = millis();
        switch (currentMenu)
        {
        case MAIN_MFD:
            Draw::RevBar();
            Draw::Gear();
            Draw::SuggestedGear();
            Draw::Speed();
            Draw::RPM();
            Draw::LapData();
            Draw::ERSbar();
            Draw::FuelBar();
            Draw::ThrottleBar();
            Renderer::finalize();
            break;

        case SETUP_MFD:
            // Todo
            // Differential and brake settings 
            break;

        case DAMAGE_MFD:
            // Todo
            // Tyre wear, tyre damage
            // Car damage
            break;

        case TEMP_MFD:
            // Todo
            // All temperatures
            break;

        case WEAR_MFD:
            // Todo
            // All wear levels of engine components
            break;

        default:
            break;
        }
        // Ensure the refresh rate is 60Hz (16.66666ms per frame)
        vTaskPrioritySet(NULL, tskIDLE_PRIORITY); // Set the priority of the task to idle
        esp_task_wdt_reset(); // Reset the watchdog timer at the end of the loop

        // Wait for the remaining time
        while (millis() - startMillis < 16)
        {
            esp_task_wdt_reset(); // Reset the watchdog timer
            delayMicroseconds(334); // Delay for a short period
            vTaskDelay(333); // Yield to allow other tasks, including the idle task, to run
        }
        delayMicroseconds(666); // Additional delay to make up for the remaining time to reach 16.66666ms
    }
    esp_task_wdt_delete(NULL);
    vTaskDelete(NULL);
}

void Renderer::init() {
    // Create sprite with dimensions
    if (!sprite.createSprite(536, 240)) {
        ESP_LOGE("Renderer", "Failed to create sprite buffer"); // Log an error message
        return;
    }
    sprite.setSwapBytes(true);
    rm67162_init(); // Initialize AMOLED LCD
    lcd_display_on();
    lcd_setRotation(1); // Set rotation to 90 degrees
    lcd_brightness(255); // Set initial brightness
}

void Renderer::finalize()
{
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer()); // Push the sprite buffer to LCD
    sprite.fillSprite(TFT_BLACK); // Clear the sprite with black color (clearing)
}