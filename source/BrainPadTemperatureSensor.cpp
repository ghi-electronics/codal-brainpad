#include "BrainPadTemperatureSensor.h"

using namespace codal;

BrainPadTemperatureSensor::BrainPadTemperatureSensor(Pin& pin, uint16_t id) : pin(pin) {

}

int BrainPadTemperatureSensor::readTemperatureCelsius() {
    return (((pin.getAnalogValue() * 3.3) - 400) / 19.5;
}
