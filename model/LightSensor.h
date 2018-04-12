#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "Pin.h"

namespace codal
{
   class LightSensor {
        Pin& pin;

    public:

        LightSensor(Pin& pin, uint16_t id);
        int readLightLevel();

   };
}

#endif
