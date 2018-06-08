#include "BrainPadLightSensor.h"

using namespace codal;

BrainPadLightSensor::BrainPadLightSensor(BrainPadPin& pin, uint16_t id) : codal::AnalogSensor(pin, id) {

}

int BrainPadLightSensor::readLightLevel() {
    return codal::AnalogSensor::getValue();
}
