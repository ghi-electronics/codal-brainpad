#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Pin.h"

namespace codal
{
   class TemperatureSensor {
        Pin& pin;
    public:

        TemperatureSensor(Pin& pin, uint16_t id);
        int readTemperatureCelsius();

   };
}

#endif
