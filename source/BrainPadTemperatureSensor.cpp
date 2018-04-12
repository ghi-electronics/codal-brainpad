#include "BrainPadTemperatureSensor.h"

using namespace codal;

BrainPadTemperatureSensor::BrainPadTemperatureSensor(Pin& pin, uint16_t id) : pin(pin){
}

int BrainPadTemperatureSensor::readTemperatureCelsius() {
    return (((pin.getAnalogValue() / 16383.0) * 3300) - 450) / 19.5;
}
