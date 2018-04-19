#include "BrainPadTemperatureSensor.h"

using namespace codal;

BrainPadTemperatureSensor::BrainPadTemperatureSensor(BrainPadPin& pin, uint16_t id) : pin(pin) {

}

int BrainPadTemperatureSensor::readTemperatureCelsius() {
    return (((pin.getAnalogValue() / 1024.0) * 3300.0) - 400.0) / 19.5;
}
