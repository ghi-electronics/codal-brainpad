#include "MMA8453.h"
#include "CodalConfig.h"
#include "ErrorNo.h"
#include "CodalCompat.h"
#include "CodalFiber.h"

using namespace codal;

// Configuration table for available g force ranges.
static const KeyValueTableEntry accelerometerRangeData[] = {
    {2, 0x00},
    {4, 0x01},
    {8, 0x02},
};

CREATE_KEY_VALUE_TABLE(accelerometerRange, accelerometerRangeData);

MMA8453::MMA8453(Pin& sda, Pin& scl, Pin &_int1, CoordinateSpace &coordinateSpace, uint16_t address,  uint16_t id) : Accelerometer(coordinateSpace, id), i2c(sda, scl), int1(_int1), sample() {
    this->id = id;
    this->status = 0;
    this->address = address;
    // Update internal state at +/- 2g.
    this->sampleRange = 2;
    // Configure and enable the accelerometer.
    configure();
}

int MMA8453::configure() {
    uint8_t value;
    WriteRegister(CTRL_REG1, 0x00);//Sleep mode
    WriteRegister(CTRL_REG2, 0x40);//Reset all registers
    WriteRegister(CTRL_REG2, 0x01);// High Resolution mode

    value = accelerometerRange.get(sampleRange);
    WriteRegister(XYZ_DATA_CFG, value);// set g mode.
    WriteRegister(CTRL_REG4, 0x01);//Enable Transient Detection Interrupt in the System. 
    WriteRegister(CTRL_REG5, 0x01);//CTRL_REG5 register to determine the routing table for the INT1
    WriteRegister(CTRL_REG1, 0x01);//Active mode

    return DEVICE_OK;
}

void MMA8453::WriteRegister(uint8_t reg, uint8_t val) {
    uint8_t writeData[2];
    writeData[0]=reg;
    writeData[1]=val;

    i2c.write(MMA8453_DEFAULT_ADDR, writeData, 2, 0); 
}

int MMA8453::whoAmI() {
    uint8_t data;
    int result;

    result = i2c.readRegister(address, WHOAMI, &data, 1);
    if (result !=0)
        return DEVICE_I2C_ERROR;

    return (int)data;
}

int MMA8453::requestUpdate() {
    updateSample();

    return DEVICE_OK;
}

int MMA8453::updateSample() {
    uint8_t readData[6];

    //check new data with interrupt pin
    if(int1.getDigitalValue() == 0) {
        // read XYZ data
        i2c.read(MMA8453_DEFAULT_ADDR, OUT_X_MSB, readData, 6);
        //read MSB and LSB values
        sample.x = readData[0] << 2 | readData[1] >> 6;
        sample.y = readData[2] << 2 | readData[3] >> 6;
        sample.z = readData[4] << 2 | readData[5] >> 6;

        // scale data to milli g. 256 counts/g for 2 g
        sample.x = (sample.x * 1000) / 256;
        if(sample.x > 1024)
            sample.x -= 4000;

        sample.y = (sample.y * 1000) / 256;
        if(sample.y > 1024)
            sample.y -= 4000;

        sample.z = (sample.z * 1000) / 256;
        if(sample.z > 1024)
            sample.z -= 4000;

        update(sample);
    }

    return DEVICE_OK;
};

int MMA8453::setPeriod(int period) {
    samplePeriod = period;

    return configure();
}

int MMA8453::getPeriod() {
    return (int)samplePeriod;
}

int MMA8453::setRange(int range) {
    sampleRange = range;

    return configure();
}

int MMA8453::getRange() {
    return (int)sampleRange;
}

Sample3D MMA8453::getSample() {
    return sample;
}

void MMA8453::idleCallback() {
    updateSample();
}

MMA8453::~MMA8453() {
}
