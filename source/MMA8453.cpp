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

static const KeyValueTableEntry periodRegisterData[] = {
    { 1,   0x39 },
    { 6,   0x31 },
    { 12,  0x29 },
    { 50,  0x21 },
    { 100, 0x19 },
    { 200, 0x11 },
    { 400, 0x09 },
    { 800, 0x01 },
};

static const KeyValueTableEntry rangeDivisorData[] = {
    { 2, 256 },
    { 4, 128 },
    { 8, 64 },
};

CREATE_KEY_VALUE_TABLE(rangeRegister, rangeRegisterData);
CREATE_KEY_VALUE_TABLE(periodRegister, periodRegisterData);
CREATE_KEY_VALUE_TABLE(rangeDivisor, rangeDivisorData);

MMA8453::MMA8453(Pin& sda, Pin& scl, Pin& int1, CoordinateSpace& coordinateSpace, uint16_t address, uint16_t id) : Accelerometer(coordinateSpace, id), i2c(sda, scl), int1(int1) {
    this->address = address;
    this->samplePeriod = 800;

    configure();
}

MMA8453::~MMA8453() {

}

void MMA8453::writeRegister(uint8_t reg, uint8_t val) {
    uint8_t data[2];

    data[0] = reg;
    data[1] = val;

    i2c.write(address, data, 2, 0);
}

int MMA8453::updateSample() {
    int divisor = rangeDivisor.get(this->getRange());
    uint8_t data[6];

    if (int1.getDigitalValue() == 0) {
        i2c.read(address, OUT_X_MSB, data, 6);

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
    writeRegister(CTRL_REG1, CTRL_REG1_SLEEP);

    writeRegister(XYZ_DATA_CFG, rangeRegister.get(getRange()));
    writeRegister(CTRL_REG4, CTRL_REG4_INT_DATA);
    writeRegister(CTRL_REG5, CTRL_REG5_SET_INT_PIN);
    writeRegister(CTRL_REG1, periodRegister.get(getPeriod()));

    return DEVICE_OK;
}

int MMA8453::requestUpdate() {
    updateSample();

    return DEVICE_OK;
}

void MMA8453::idleCallback() {
    updateSample();
}
