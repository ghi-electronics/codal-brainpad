#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "BrainPadPin.h"
#include "AnalogSensor.h"

enum class LightCondition {
    //% block="dark"
    Dark = ANALOG_THRESHOLD_LOW,
    //% block="bright"
    Bright = ANALOG_THRESHOLD_HIGH
};


namespace codal
{
   class LightSensor : public codal::AnalogSensor {

    public:
  
      LightSensor(Pin &pin, uint16_t id);
   
   };
}

#endif