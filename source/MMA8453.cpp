#include "MMA8453.h"
#include "CodalConfig.h"
#include "ErrorNo.h"
#include "CodalCompat.h"
#include "CodalFiber.h"

using namespace codal;

static const KeyValueTableEntry accelerometerRangeData[] = {
    {2, 0x00},
    {4, 0x01},
    {8, 0x02},
};

CREATE_KEY_VALUE_TABLE(accelerometerRange, accelerometerRangeData);

MMA8453::MMA8453(Pin& sda, Pin& scl, Pin& int1, CoordinateSpace &coordinateSpace, uint16_t address, uint16_t id) : Accelerometer(coordinateSpace, id), i2c(sda, scl), int1(int1), sample() {
    this->id = id;
    this->status = 0;
    this->address = address;
    this->sampleRange = 2;
    configure();
}

int MMA8453::configure() {
    writeRegister(CTRL_REG1, CTRL_REG1_SLEEP);
    writeRegister(CTRL_REG2, CTRL_REG2_RESET);
    writeRegister(CTRL_REG2, CTRL_REG2_HIGH_RES);

    uint8_t value = accelerometerRange.get(sampleRange);
    writeRegister(XYZ_DATA_CFG, value);
    writeRegister(CTRL_REG4, CTRL_REG4_INT_DATA);
    writeRegister(CTRL_REG5, CTRL_REG5_SET_INT_PIN);
    writeRegister(CTRL_REG1, CTRL_REG1_ACTIVE);

    return DEVICE_OK;
}

void MMA8453::writeRegister(uint8_t reg, uint8_t val) {
    uint8_t writeData[2];
    writeData[0] = reg;
    writeData[1] = val;

    i2c.write(MMA8453_DEFAULT_ADDR, writeData, 2, 0); 
}

int MMA8453::requestUpdate() {
    updateSample();

    return DEVICE_OK;
}

int MMA8453::updateSample() {
    uint8_t readData[6];
    int sensetivity, divisor, offset;

    switch (this->getRange()) {
        default: sensetivity = 256; divisor = 2048; offset = 4096; break;
        case 2: sensetivity = 256; divisor = 2048; offset = 4096; break;
        case 4: sensetivity= 128; divisor = 4096; offset = 8192; break;
        case 8: sensetivity= 64; divisor = 8192; offset = 16384; break;
    }

    if(int1.getDigitalValue() == 0) {
        i2c.read(MMA8453_DEFAULT_ADDR, OUT_X_MSB, readData, 6);

        sample.x = (readData[0] << 2) | (readData[1] >> 6);
        sample.y = (readData[2] << 2) | (readData[3] >> 6);
        sample.z = (readData[4] << 2) | (readData[5] >> 6);

        sample.x = (sample.x * 1000) / sensetivity;
        if(sample.x > divisor)
            sample.x -= offset;

        sample.y = (sample.y * 1000) / sensetivity;
        if(sample.y > divisor)
            sample.y -= offset;

        sample.z = (sample.z * 1000) / sensetivity;
        if(sample.z > divisor)
            sample.z -= offset;
        update(sample);
    }

    return DEVICE_OK;
};

void MMA8453::idleCallback() {
    updateSample();
}

MMA8453::~MMA8453() {
}
