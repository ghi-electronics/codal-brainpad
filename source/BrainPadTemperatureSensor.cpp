#include "BrainPadTemperatureSensor.h"

using namespace codal;

BrainPadTemperatureSensor::BrainPadTemperatureSensor(BrainPadPin& pin, uint16_t id) : codal::AnalogSensor(pin, id) {

}

int BrainPadTemperatureSensor::getValue() {
    return (((sensor->getValue() / 1024.0) * 3300.0) - 450.0) / 19.5;
}
