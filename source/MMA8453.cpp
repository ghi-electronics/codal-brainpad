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

void MMA8453::writeRegister(uint8_t reg, uint8_t val) {
    uint8_t data[2];

    data[0] = reg;
    data[1] = val;

    i2c.write(address, data, 2, 0);
}

int MMA8453::updateSample() {
    int divisor = rangeDivisor.get(this->getRange());
    uint8_t data[6];
	int1.getDigitalValue();

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
            uint8_t buffer[1];
			writeRegister(0x2B, 0x40);
			// Waits until reset bit is cleared
			do {
				i2c.readRegister(address, 0x2B, buffer, 1);
			} while((buffer[0] & 0x40) >> 6 == 1);

            writeRegister(0x2C, 0x02); // CTRL_REG3 Set interrupt to Active High
            writeRegister(0x15, 0x78); // FF_MT_CFG setting Motion Detection on all three axis
            writeRegister(0x17, 0x0F); // FF_MT_THS Sets threshhold
            writeRegister(0x2D, 0x04); // CTRL_REG4
            writeRegister(0x2E, 0x04); // CTRL_REG5
            writeRegister(0x2A, 0x01); // CTRL_REG1 activate

    return DEVICE_OK;
}

int MMA8453::requestUpdate() {
    updateSample();

    return DEVICE_OK;
}

void MMA8453::idleCallback() {
    updateSample();
}