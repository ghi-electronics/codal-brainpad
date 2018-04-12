#ifndef BRAINPADTEMPERATURESENSOR_H
#define BRAINPADTEMPERATURESENSOR_H

#include "Pin.h"

namespace codal
{
   class BrainPadTemperatureSensor {
        Pin& pin;
    public:

        BrainPadTemperatureSensor(Pin& pin, uint16_t id);
        int readTemperatureCelsius();

   };
}

#endif
