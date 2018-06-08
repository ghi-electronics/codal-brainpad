#ifndef BPTEMPERATURESENSOR_H
#define BPTEMPERATURESENSOR_H

#include "BrainPadPin.h"
#include "AnalogSensor.h"

namespace codal {
    class BrainPadTemperatureSensor : public codal::AnalogSensor {
        codal::AnalogSensor* sensor = this;

    public:
        BrainPadTemperatureSensor(BrainPadPin& pin, uint16_t id);

        int getValue();
    };
}

#endif
