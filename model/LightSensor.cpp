#include "LightSensor.h"

using namespace codal;

LightSensor::LightSensor(Pin& pin, uint16_t id) : pin(pin) {
}

int LightSensor::readLightLevel() {
    int value = pin.getAnalogValue() / 16;

    return value;
}
