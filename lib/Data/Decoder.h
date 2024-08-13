#ifndef DECODER_H
#define DECODER_H
#include <Data.h>
#include <ArduinoJson.h>
#include <cstdlib>
#include <cstring>

namespace Decoder {
  /**
   * The async loop for the decoder. This function must be ran on ``CORE 1``.
   * @note This function will be ran in an infinite loop. Use xTaskCreatePinnedToCore to run this function on ``CORE 1``.
   * @param params The parameters to pass to the function (not used)
   */
  void loop();

  /**
   * Construct a packet from the raw data.
   * @param rawData A pointer to the raw data
   */
  void constructPacket(char *rawData);

  /**
   * Decode a packet.
   * @param packet The packet to decode
   */
  void decodePacket(IncomingPacket packet);

  /**
   * Data is sent per type. This function will decode the data for the telemetry of the car (speed RPM etc.).
   * @param packetContents The packet containing the player data
   */
  void decodeTelemetry(String packetContents);

  /**
   * Data is sent per type. This function will decode the data for the session data (position).
   * @param packetContents The packet containing the session data
   */
  void decodeSessiondata(String packetContents);

  /**
   * Data is sent per type. This function will decode the data for the lap data.
   * @param packetContents The packet containing the event data
   */
  void decodeLapdata(String packetContents);

  /**
   * Data is sent per type. This function will decode the data for the car status.
   * @param packetContents The packet containing the event data
   */
  void decodeCarstatus(String packetContents);
}

#endif