#include "F1WiFiUDP.hpp"

F1WiFiUDP::F1WiFiUDP()
{
    UDPBufferMutex = xSemaphoreCreateMutex();
    status = WL_IDLE_STATUS;
    nvs.begin();
}

F1WiFiUDP::~F1WiFiUDP()
{
    vSemaphoreDelete(UDPBufferMutex);
    nvs.close();
    udpTaskRunning = false;
    if (udpTaskHandle != NULL)
    {
        vTaskDelete(udpTaskHandle);
    }
}

uint8_t F1WiFiUDP::connectWlan()
{
    if (status == WL_CONNECTED)
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

    status = WL_CONNECTED;
    return 0;
}

bool F1WiFiUDP::beginUDP(uint16_t port)
{
    if (status != WL_CONNECTED) return false;
    bool ok = udp.begin(port);
    if (ok)
    {
        udpTaskRunning = true;
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
    while (udpTaskRunning)
    {
        if (classPtr->status == WL_CONNECTED)
        {
            size_t packetSize = classPtr->udp.parsePacket();
            if (packetSize)
            {
                xSemaphoreTake(classPtr->UDPBufferMutex, portMAX_DELAY);

                // Allocate dynamic buffer based on packet size
                if (classPtr->UDPBuffer != nullptr)
                {
                    free(classPtr->UDPBuffer);
                }
                classPtr->UDPBuffer = (uint8_t *)malloc(packetSize + 1);
                if (classPtr->UDPBuffer == nullptr)
                {
                    Serial.println("Failed to allocate memory for UDP buffer");
                    xSemaphoreGive(classPtr->UDPBufferMutex);
                    continue;
                }

                // Read the packet into the buffer
                classPtr->UDPBufferSize = classPtr->udp.read(classPtr->UDPBuffer, packetSize);
                classPtr->UDPBuffer[classPtr->UDPBufferSize] = '\0'; // Null-terminate the string
                classPtr->UDPBufferEmpty = false;

                // Flush the UDP buffer
                classPtr->udp.flush();
                xSemaphoreGive(classPtr->UDPBufferMutex);
            }
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}