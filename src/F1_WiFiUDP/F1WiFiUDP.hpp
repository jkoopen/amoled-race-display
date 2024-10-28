#pragma once

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoNvs.h>

class F1WiFiUDP
{
public:
    F1WiFiUDP();
    ~F1WiFiUDP();

    uint8_t connectWlan(); // 0 = OK, 1 = ERROR, 2 = NO CREDENTIALS
    bool beginUDP(uint16_t port);

    bool resetCredentials();
    bool storeCredentials(String ssid, String password);

    static void udpTask(F1WiFiUDP *classPtr);

    SemaphoreHandle_t UDPBufferMutex;
    uint8_t *UDPBuffer;
    size_t UDPBufferSize;
    bool UDPBufferEmpty;

    wl_status_t status;
    TaskHandle_t udpTaskHandle;
    static volatile bool udpTaskRunning;
private:
    WiFiUDP udp;
    ArduinoNvs nvs;
};