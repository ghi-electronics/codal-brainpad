#include "BrainPadLightSensor.h"

using namespace codal;

BrainPadLightSensor::BrainPadLightSensor(BrainPadPin& pin, uint16_t id) : pin(pin) {

}

int BrainPadLightSensor::readLightLevel() {
    return pin.getAnalogValue() >> 2;
}
