#ifndef MMA8453_H
#define MMA8453_H

#include "CodalConfig.h"
#include "CodalComponent.h"
#include "CodalUtil.h"
#include "CoordinateSystem.h"
#include "Pin.h"
#include "MbedI2C.h"
#include "Accelerometer.h"
#include "BrainPadIO.h"

#define MMA8453_DEFAULT_ADDR    0x38
#define MMA8453_WHOAMI_VAL      0x3A

//Register map (partial)
#define STATUS_REG       0x00
#define STATUS_REG_AUX   0x07
#define OUT_X_MSB        0x01
#define OUT_X_LSB        0x02
#define OUT_Y_MSB        0x03
#define OUT_Y_LSB        0x04
#define OUT_Z_MSB        0x05
#define OUT_Z_LSB        0x06
#define WHOAMI           0x0D

#define CTRL_REG1        0x2A
#define CTRL_REG2        0x2B
#define CTRL_REG3        0x2C
#define CTRL_REG4        0x2D
#define CTRL_REG5        0x2E
#define TRANSIENT_CFG    0x1D
#define TRANSIENT_THS    0x1F
#define TRANSIENT_COUNT  0x20
#define HP_FILTER_CUTOFF 0x0F
#define FT_MT_THS        0x17
#define FF_MT_COUNT      0x18
#define FF_MT_CFG        0x15
#define XYZ_DATA_CFG     0x0E

namespace codal {
    class MMA8453 : public Accelerometer {
        codal::_mbed::I2C i2c;  
        Pin             &int1;              // Data ready interrupt.
        uint16_t        address;            // I2C address of this accelerometer.
        uint16_t        samplePeriod;       // The time between samples, in milliseconds.
        uint8_t         sampleRange;        // The sample range of the accelerometer in g.
        Sample3D        sample;             // The last sample read.

    public:
        MMA8453(Pin& sda, Pin& scl, Pin &_int1, CoordinateSpace &coordinateSpace, uint16_t address = MMA8453_DEFAULT_ADDR, uint16_t id = DEVICE_ID_ACCELEROMETER);

        int setPeriod(int period);
        virtual int getPeriod();
        int setRange(int range);
        virtual int getRange();
        int whoAmI();

        Sample3D getSample();
        void WriteRegister(uint8_t, uint8_t);
        virtual void idleCallback();
        int configure();
        int updateSample();
        virtual int requestUpdate();

        ~MMA8453();
    };
}

#endif
