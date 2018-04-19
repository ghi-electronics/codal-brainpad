#ifndef BPTEMPERATURESENSOR_H
#define BPTEMPERATURESENSOR_H

#include "BrainPadPin.h"

namespace codal {
    class BrainPadTemperatureSensor {
        BrainPadPin& pin;

    public:
        BrainPadTemperatureSensor(BrainPadPin& pin, uint16_t id);

        int readTemperatureCelsius();
    };
}

#endif
