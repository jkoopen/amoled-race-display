#include "Renderer.h"

void Renderer::refresh(void *pvParameters)
{
    //esp_task_wdt_add(NULL); // Add this task to the watchdog timer
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Initialize the last wake time
    vTaskDelay(1); // Delay for 1ms to ensure the task is ready

    // Print the core ID on which the refresh task is running
    Serial.printf("Refresh task running on core %d\n", xPortGetCoreID());

    while (true) // Loop forever
    {
        esp_task_wdt_reset(); // Reset the watchdog timer at the beginning of the loop
        uint64_t currentMillis = millis();
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

        // Ensure the refresh rate is 60Hz (16ms per frame)
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(16)); // Delay until the next cycle
        esp_task_wdt_reset(); // Reset the watchdog timer at the end of the loop
    }

    esp_task_wdt_delete(NULL); // Clean up, if the loop ever exits
}

void Renderer::finalize()
{
    esp_task_wdt_add(NULL); // Add this task to the watchdog timer
    esp_task_wdt_reset(); // Reset the watchdog timer
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer()); // Push the sprite buffer to LCD
    sprite.fillSprite(TFT_BLACK); // Clear the sprite with black color
    esp_task_wdt_reset(); // Reset the watchdog timer
}