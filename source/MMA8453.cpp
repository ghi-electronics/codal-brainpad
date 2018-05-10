#include "MMA8453.h"
#include "CodalConfig.h"
#include "ErrorNo.h"
#include "CodalCompat.h"
#include "CodalFiber.h"

using namespace codal;

static const KeyValueTableEntry rangeRegisterData[] = {
    { 2, 0x00 },
    { 4, 0x01 },
    { 8, 0x02 },
};

static const KeyValueTableEntry rangeDivisorData[] = {
    { 2, 256 },
    { 4, 128 },
    { 8, 64 },
};

CREATE_KEY_VALUE_TABLE(rangeRegister, rangeRegisterData);
CREATE_KEY_VALUE_TABLE(rangeDivisor, rangeDivisorData);

MMA8453::MMA8453(codal::I2C& _i2c, Pin& _int1, CoordinateSpace& coordinateSpace, uint16_t address, uint16_t id) : Accelerometer(coordinateSpace, id), i2c(_i2c), int1(_int1) {

    this->address = address;

    configure();
}

MMA8453::~MMA8453() {

}

int MMA8453::updateSample() {
    int divisor = rangeDivisor.get(this->getRange());
    uint8_t data[6];

    if (int1.getDigitalValue() == 0) {
        i2c.readRegister(address, OUT_X_MSB, data, 6);

        int32_t x = (data[0] << 2) | (data[1] >> 6);
        int32_t y = (data[2] << 2) | (data[3] >> 6);
        int32_t z = (data[4] << 2) | (data[5] >> 6);

        if (x >= 512) x -= 1024;
        if (y >= 512) y -= 1024;
        if (z >= 512) z -= 1024;

        x *= 1024;
        x /= divisor;

        y *= 1024;
        y /= divisor;

        z *= 1024;
        z /= divisor;

        update({ -y, x, -z }); //To transform to ENU
    }

    return DEVICE_OK;
}

int MMA8453::configure() {
    i2c.writeRegister(address, CTRL_REG1, CTRL_REG1_SLEEP);

    i2c.writeRegister(address, XYZ_DATA_CFG, rangeRegister.get(getRange()));
    i2c.writeRegister(address, CTRL_REG4, CTRL_REG4_INT_DATA);
    i2c.writeRegister(address, CTRL_REG5, CTRL_REG5_SET_INT_PIN);
    i2c.writeRegister(address, CTRL_REG1, CTRL_REG1_ACTIVE);

    return DEVICE_OK;
}

int MMA8453::requestUpdate() {
    updateSample();

    return DEVICE_OK;
}

void MMA8453::idleCallback() {
    updateSample();
}
