#ifndef HARDWARE_H
#define HARDWARE_H

#include <config.h>
#include <Esp.h>
#include <Renderer.h>
#include <Decoder.h>
#include <Data.h>

namespace Hardware {
    /**************************************************************************
     * @brief Initialize and setup all other hardware related functions.
     * @param None
     *************************************************************************/
    void init(uint32_t baud = 115200);

    /**************************************************************************
     * @brief Cycle the brightness of display when the button is pressed.
     * @param None
     *************************************************************************/
    void cycleBrightness();
}

#endif // HARDWARE_H