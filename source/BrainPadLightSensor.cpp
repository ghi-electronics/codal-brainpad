#include "BrainPadLightSensor.h"

using namespace codal;

BrainPadLightSensor::BrainPadLightSensor(Pin& pin, uint16_t id) : pin(pin) {

}

int BrainPadLightSensor::readLightLevel() {
    return pin.getAnalogValue();
}
