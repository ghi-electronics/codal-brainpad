#ifndef BPLIGHTSENSOR_H
#define BPLIGHTSENSOR_H

#include "BrainPadPin.h"

namespace codal {
    class BrainPadLightSensor {
        BrainPadPin& pin;

    public:
        BrainPadLightSensor(BrainPadPin& pin, uint16_t id);

        int readLightLevel();
    };
}

#endif
