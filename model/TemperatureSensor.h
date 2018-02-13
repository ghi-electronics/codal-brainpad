#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H


#include "BrainPadPin.h"
#include "AnalogSensor.h"

enum class TemperatureCondition {
    //% block="hot"
    Hot = ANALOG_THRESHOLD_HIGH,
    //% block="cold"
    Cold = ANALOG_THRESHOLD_LOW
};

enum class TemperatureUnit {
    //% block="°C"
    Celsius,
    //% block="°F"
    Fahrenheit
};


namespace codal
{
   class TemperatureSensor : public codal::AnalogSensor {

    public:
  
      TemperatureSensor(Pin &pin, uint16_t id);
   
   };
}

#endif