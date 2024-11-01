#include "F1Parser.hpp"

F1Parser::F1Parser(F1WiFiUDP *udpPtr)
{
    udp = udpPtr;
}

F1Parser::~F1Parser()
{
    udp = nullptr;
    parseTaskRunning = false;
    vTaskDelete(parseTaskHandle);
}

void F1Parser::start()
{
    parseTaskArgs *args = new parseTaskArgs;
    args->udp = this->udp;
    args->parser = this;
    parseTaskRunning = true;
    xTaskCreate((TaskFunction_t)parseTask, "parseTask", 4096, args, 1, &parseTaskHandle);
}

// Convert a 16-bit value from Little Endian to host endianness
uint16_t le16toh(uint16_t val) {
    return (val << 8) | (val >> 8);
}

// Convert a 32-bit value from Little Endian to host endianness
uint32_t le32toh(uint32_t val) {
    return ((val << 24) & 0xFF000000) |
           ((val << 8) & 0x00FF0000) |
           ((val >> 8) & 0x0000FF00) |
           ((val >> 24) & 0x000000FF);
}

// Convert a 64-bit value from Little Endian to host endianness
uint64_t le64toh(uint64_t val) {
    return ((val << 56) & 0xFF00000000000000ULL) |
           ((val << 40) & 0x00FF000000000000ULL) |
           ((val << 24) & 0x0000FF0000000000ULL) |
           ((val << 8) & 0x000000FF00000000ULL) |
           ((val >> 8) & 0x00000000FF000000ULL) |
           ((val >> 24) & 0x0000000000FF0000ULL) |
           ((val >> 40) & 0x000000000000FF00ULL) |
           ((val >> 56) & 0x00000000000000FFULL);
}

void F1Parser::parseTask(parseTaskArgs *args)
{
    F1WiFiUDP *udp = args->udp;
    F1Parser *parser = args->parser;

    while (parser->parseTaskRunning)
    {
        // Lock the UDP buffer mutex
        if (xSemaphoreTake(udp->UDPBufferMutex, portMAX_DELAY) == pdTRUE)
        {
            if (!udp->UDPBufferEmpty)
            {
                // Parse the packet header
                PacketHeader *header = reinterpret_cast<PacketHeader*>(udp->UDPBuffer.data());

                // Convert header fields from Little Endian to host endianness
                header->m_packetFormat = le16toh(header->m_packetFormat);
                header->m_sessionUID = le64toh(header->m_sessionUID);
                header->m_frameIdentifier = le32toh(header->m_frameIdentifier);

                // Check the packet format and version
                if (header->m_packetFormat == 2022)
                {
                    switch (header->m_packetId)
                    {
                    case 0: // Motion packet
                        parser->parseMotionPacket(parser);
                        break;
                    case 1: // Session packet
                        parser->parseSessionPacket(parser);
                        break;
                    case 2: // Lap data packet
                        parser->parseLapPacket(parser);
                        break;
                    case 3: // Event packet
                        parser->parseEventPacket(parser);
                        break;
                    case 4: // Participants packet
                        parser->parseParticipantsPacket(parser);
                        break;
                    case 5: // Car setup packet
                        parser->parseCarSetupPacket(parser);
                        break;
                    case 6: // Telemetry packet
                        parser->parseTelemetryPacket(parser);
                        break;
                    case 7: // Status packet
                        parser->parseStatusPacket(parser);
                        break;
                    default:
                        Serial.printf("Unknown packet ID: %d\n", header->m_packetId);
                        break;
                    }
                }

                // Mark the buffer as empty
                udp->UDPBufferEmpty = true;
            }

            // Release the UDP buffer mutex
            xSemaphoreGive(udp->UDPBufferMutex);
        }

        // Small delay to avoid overwhelming the serial output
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}