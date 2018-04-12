#include "TemperatureSensor.h"

using namespace codal;

TemperatureSensor::TemperatureSensor(Pin& pin, uint16_t id) : pin(pin){
}

int TemperatureSensor::readTemperatureCelsius() {
    int value = (((pin.getAnalogValue() / 16383.0) * 3300) - 450) / 19.5;

    return value;
}
