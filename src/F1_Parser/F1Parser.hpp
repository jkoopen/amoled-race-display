#pragma once
#include "F1WiFiUDP.hpp"
#include "F1Structs.hpp"

struct parseTaskArgs {
    F1WiFiUDP* udp;
    F1Parser* parser;
};

class F1Parser
{
public:
    F1Parser(F1WiFiUDP *udpPtr);
    ~F1Parser();

    void start();

    static void parseTask(parseTaskArgs *args);
    TaskHandle_t parseTaskHandle;
    static volatile bool parseTaskRunning;

    // Declare instances of all packet structures, for other classes to use
    PacketMotionData motionData;
    PacketSessionData sessionData;
    PacketLapData lapData;
    PacketEventData eventData;
    PacketParticipantsData participantsData;
    PacketCarSetupData carSetupData;
    PacketCarTelemetryData telemetryData;
    PacketCarStatusData statusData;


private:
    F1WiFiUDP *udp;
    void parseMotionPacket(F1Parser *parser);
    void parseSessionPacket(F1Parser *parser);
    void parseLapPacket(F1Parser *parser);
    void parseEventPacket(F1Parser *parser);
    void parseParticipantsPacket(F1Parser *parser);
    void parseCarSetupPacket(F1Parser *parser);
    void parseTelemetryPacket(F1Parser *parser);
    void parseStatusPacket(F1Parser *parser);
};