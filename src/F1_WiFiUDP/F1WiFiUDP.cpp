#include "F1WiFiUDP.hpp"

F1WiFiUDP::F1WiFiUDP()
{
    UDPBufferMutex = xSemaphoreCreateMutex();
    nvs.begin();
}

F1WiFiUDP::~F1WiFiUDP()
{
    vSemaphoreDelete(UDPBufferMutex);
    nvs.close();
    this->udpTaskRunning = false;
    if (udpTaskHandle != NULL)
    {
        vTaskDelete(udpTaskHandle);
    }
}

uint8_t F1WiFiUDP::connectWlan()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return 0;
    }

    const char *ssid = nvs.getString("ssid").c_str();
    const char *password = nvs.getString("password").c_str();
    if (ssid == NULL || password == NULL)
    {
        return 2;
    }

    WiFi.begin(ssid, password);
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start > 10000)
        {
            return false;
        }
        delay(100);
    }

    return 0;
}

bool F1WiFiUDP::beginUDP(uint16_t port)
{
    if (WiFi.status() != WL_CONNECTED)
        return false;
    bool ok = udp.begin(port);
    if (ok)
    {
        this->udpTaskRunning = true;
        xTaskCreate((TaskFunction_t)udpTask, "udpTask", 4096, this, 1, &udpTaskHandle);
    }
    return true;
}

bool F1WiFiUDP::resetCredentials()
{
    return nvs.erase("ssid") && nvs.erase("password");
}

bool F1WiFiUDP::storeCredentials(String ssid, String password)
{
    return nvs.setString("ssid", ssid) && nvs.setString("password", password);
}

void F1WiFiUDP::udpTask(F1WiFiUDP *classPtr)
{
    while (classPtr->udpTaskRunning)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        int packetSize = classPtr->udp.parsePacket();
        if (packetSize)
        {
            xSemaphoreTake(classPtr->UDPBufferMutex, portMAX_DELAY);

            // Allocate dynamic buffer based on packet size
            classPtr->UDPBuffer.resize(packetSize + 1);

            // Read the packet into the buffer
            classPtr->UDPBufferSize = classPtr->udp.read(classPtr->UDPBuffer.data(), packetSize);
            classPtr->UDPBuffer[classPtr->UDPBufferSize] = '\0'; // Null-terminate the string
            classPtr->UDPBufferEmpty = false;

            xSemaphoreGive(classPtr->UDPBufferMutex);
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

void F1WiFiUDP::credentialsChecker(F1WiFiUDP *classPtr)
{
    if (Serial.available())
    {
        static char message[256]; // Create a buffer for the message
        static uint8_t messagePosition = 0;      // Create a variable to store the message position

        while (Serial.available() > 0)
        {
            char incomingByte = Serial.read(); // Grab the incoming byte

            if (incomingByte != '\n' /* LINE FEED*/ && (messagePosition < 256 - 1))
            {
                message[messagePosition] = incomingByte; // Add the incoming byte to the message
                messagePosition++;                       // Increment the message position
            }
            else
            {
                message[messagePosition] = '\0'; // Null-terminate the string (empty charset)
                messagePosition = 0;             // Reset the message position

                // Split the message into ssid and password
                char *ssid = strtok(message, "|");

                if (ssid != NULL)
                {
                    char *password = strtok(NULL, "|");
                    if (password != NULL)
                    {
                        if (strcmp(ssid, DEBUG_REBOOT_KEY) == 0 && strcmp(password, DEBUG_REBOOT_KEY) == 0)
                        {
                            ESP.restart();
                        }
                        else if (strcmp(ssid, DEBUG_RESET_KEY) == 0 && strcmp(password, DEBUG_RESET_KEY) == 0)
                        {
                            classPtr->resetCredentials();
                            Serial.println("Credentials reset!");
                        }
                        else
                        {
                            classPtr->storeCredentials(String(ssid), String(password));
                            Serial.println("Credentials stored!");
                        }
                    }
                }
            }
        }
    }
}