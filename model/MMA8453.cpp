#include "MMA8453.h"
#include "CodalConfig.h"
#include "ErrorNo.h"
#include "CodalCompat.h"
#include "CodalFiber.h"

using namespace codal;

//
// Configuration table for available g force ranges.
// Maps g -> LIS3DH_CTRL_REG4 [5..4]
//
static const KeyValueTableEntry accelerometerRangeData[] = {
    {2, 0},
    {4, 1},
    {8, 2},
 
};
CREATE_KEY_VALUE_TABLE(accelerometerRange, accelerometerRangeData);


/**
  * Constructor.
  * Create a software abstraction of an accelerometer.
  *
  * @param _i2c an instance of DeviceI2C used to communicate with the onboard accelerometer.
  * @param _int1 the pin connected to the LIS3DH IRQ line.
  * @param coordinateSpace The orientation of the sensor.
  * @param address the default I2C address of the accelerometer. Defaults to: LIS3DH_DEFAULT_ADDR.
  * @param id the unique EventModel id of this component. Defaults to: DEVICE_ID_ACCELEROMETER
  *
  * @code
  * DeviceI2C i2c = DeviceI2C(I2C_SDA0, I2C_SCL0);
  *
  * LIS3DH accelerometer = LIS3DH(i2c);
  * @endcode
 */
MMA8453::MMA8453(I2C& _i2c, Pin &_int1, CoordinateSpace &coordinateSpace, uint16_t address,  uint16_t id) : Accelerometer(coordinateSpace, id), i2c(_i2c), int1(_int1), sample()
{
    // Store our identifiers.
    this->id = id;
    this->status = 0;
    this->address = address;

    // Update our internal state for 50Hz at +/- 2g (50Hz has a period af 20ms).
    this->samplePeriod = 20;
    this->sampleRange = 2;

    // Configure and enable the accelerometer.
    configure();
}

/**
  * Configures the accelerometer for G range and sample rate defined
  * in this object. The nearest values are chosen to those defined
  * that are supported by the hardware. The instance variables are then
  * updated to reflect reality.
  *
  * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the accelerometer could not be configured.
  */
int MMA8453::configure()
{
    // STANDBY Mode
    i2c.writeRegister(address, CTRL_REG1, 0x18); 
    
    //The TRANSIENT_CFG register is used to enable the transient interrupt generation mechanism for the 3 axes (X, Y, Z) of acceleration.
    i2c.writeRegister(address, TRANSIENT_CFG, 0x16);

    //The Transient Threshold register sets the threshold limit for the detection of the transient acceleration. max 8g. for test
    i2c.writeRegister(address, TRANSIENT_THS, 0x08);

   // Set the Debounce Counter for 50 ms: Register 0x20
    i2c.writeRegister(address, 0x20, 0x05);

    //Enable Transient Detection Interrupt in the System. 
    i2c.writeRegister(address, CTRL_REG4, 0x20);

    //CTRL_REG5 register to determine the routing table for the INT1 and INT2 interrupt pins. INT1 - 0x01, INT2 - 0x00
    i2c.writeRegister(address, CTRL_REG5, 0x20);

    // Put the device in Active Mode: Register 0x2A CTRL_REG1
    i2c.writeRegister(address, CTRL_REG1, 0x01); 

      
    
    return DEVICE_OK;
}


/**
  * Attempts to read the 8 bit ID from the accelerometer, this can be used for
  * validation purposes.
  *
  * @return the 8 bit ID returned by the accelerometer, or DEVICE_I2C_ERROR if the request fails.
  *
  * @code
  * accelerometer.whoAmI();
  * @endcode
  */
int MMA8453::whoAmI()
{
    uint8_t data;
    int result;

    result = i2c.readRegister(address, WHOAMI, &data, 1);
    if (result !=0)
        return DEVICE_I2C_ERROR;

    return (int)data;
}

/**
 * Poll to see if new data is available from the hardware. If so, update it.
 * n.b. it is not necessary to explicitly call this funciton to update data
 * (it normally happens in the background when the scheduler is idle), but a check is performed
 * if the user explicitly requests up to date data.
 *
 * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the update fails.
 *
 * @note This method should be overidden by the hardware driver to implement the requested
 * changes in hardware.
 */
int MMA8453::requestUpdate()
{
    updateSample();
    return DEVICE_OK;
}

/**
  * Reads the acceleration data from the accelerometer, and stores it in our buffer.
  * This only happens if the accelerometer indicates that it has new data via int1.
  *
  * On first use, this member function will attempt to add this component to the
  * list of fiber components in order to constantly update the values stored
  * by this object.
  *
  * This technique is called lazy instantiation, and it means that we do not
  * obtain the overhead from non-chalantly adding this component to fiber components.
  *
  * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the read request fails.
  */
int MMA8453::updateSample()
{
    int8_t data[6];
    uint8_t src;
    int result;

   if(int1.getDigitalValue() == 1)
    {
       i2c.writeRegister(address, OUT_X_MSB, 0x01);
       result = i2c.readRegister(address, OUT_X_MSB, (uint8_t *)data, 6);

        sample.x = data[0] << 2 | data[1] >> 6;
        sample.y = data[2] << 2 | data[3] >> 6;
        sample.z = data[4] << 2 | data[5] >> 6;
 
        update(sample);

    }

    return DEVICE_OK;
};



/**
  * Attempts to set the sample rate of the accelerometer to the specified value (in ms).
  *
  * @param period the requested time between samples, in milliseconds.
  *
  * @return DEVICE_OK on success, DEVICE_I2C_ERROR is the request fails.
  *
  * @code
  * // sample rate is now 20 ms.
  * accelerometer.setPeriod(20);
  * @endcode
  *
  * @note The requested rate may not be possible on the hardware. In this case, the
  * nearest lower rate is chosen.
  */
int MMA8453::setPeriod(int period)
{
    samplePeriod = period;
    return configure();
}

/**
  * Reads the currently configured sample rate of the accelerometer.
  *
  * @return The time between samples, in milliseconds.
  */
int MMA8453::getPeriod()
{
    return (int)samplePeriod;
}

/**
  * Attempts to set the sample range of the accelerometer to the specified value (in g).
  *
  * @param range The requested sample range of samples, in g.
  *
  * @return DEVICE_OK on success, DEVICE_I2C_ERROR is the request fails.
  *
  * @code
  * // the sample range of the accelerometer is now 8G.
  * accelerometer.setRange(8);
  * @endcode
  *
  * @note The requested range may not be possible on the hardware. In this case, the
  * nearest lower range is chosen.
  */
int MMA8453::setRange(int range)
{
    sampleRange = range;
    return configure();
}

/**
  * Reads the currently configured sample range of the accelerometer.
  *
  * @return The sample range, in g.
  */
int MMA8453::getRange()
{
    return (int)sampleRange;
}

/**
 * Reads the accelerometer data from the latest update retrieved from the accelerometer.
 * Data is provided in ENU format, relative to the device package (and makes no attempt
 * to align axes to the device).
 *
 * @return The force measured in each axis, in milli-g.
 *
 */
Sample3D MMA8453::getSample()
{
    return sample;
}


/**
  * A periodic callback invoked by the fiber scheduler idle thread.
  *
  * Internally calls updateSample().
  */
void MMA8453::idleCallback()
{
    updateSample();
}

/**
  * Destructor for MMA8453, where we deregister from the array of fiber components.
  */
MMA8453::~MMA8453()
{
}
