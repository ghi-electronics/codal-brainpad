#include "TemperatureSensor.h"
#include "BrainPadPin.h"

using namespace codal;


TemperatureSensor::TemperatureSensor(Pin &pin, uint16_t id) : codal::AnalogSensor(pin, id)
    
         
{
  TemperatureSensor::init();
  TemperatureSensor::updateSample();
}

