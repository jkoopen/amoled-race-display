#pragma once

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoNvs.h>
#include <ArduinoJson.h>
#include "main.hpp"

#define DEBUG_RESET_KEY "r3s3t"
#define DEBUG_REBOOT_KEY "r3b00t"

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

    static void credentialsChecker(F1WiFiUDP *classPtr);

    SemaphoreHandle_t UDPBufferMutex;
    std::vector<uint8_t> UDPBuffer;
    size_t UDPBufferSize;
    bool UDPBufferEmpty;

    TaskHandle_t udpTaskHandle;
    volatile bool udpTaskRunning;
private:
    WiFiUDP udp;
    ArduinoNvs nvs;
};