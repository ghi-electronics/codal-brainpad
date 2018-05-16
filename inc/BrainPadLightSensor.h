#ifndef BPLIGHTSENSOR_H
#define BPLIGHTSENSOR_H

#include "BrainPadPin.h"
#include "AnalogSensor.h"

namespace codal {
    class BrainPadLightSensor : public codal::AnalogSensor{
        codal::AnalogSensor* sensor = this;

    public:
        BrainPadLightSensor(BrainPadPin& pin, uint16_t id);

        int readLightLevel();
    };
}

#endif
