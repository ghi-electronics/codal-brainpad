#include "BrainPad.h"

int temperature(TemperatureUnit unit);
int lightLevel();

BrainPad brain;

int
main()
{
    brain.init();
    brain.lcd.InitScreen();
    
    while(1)
    {
        
        int temper = temperature(TemperatureUnit::Celsius);
        int light = lightLevel();

        if(temper > 27)
           brain.io.ledRed.setDigitalValue(1);
        else brain.io.ledRed.setDigitalValue(0);
  
        if(light > 150)
           brain.io.ledGreen.setDigitalValue(1);
        else brain.io.ledGreen.setDigitalValue(0);
       
    }
}

/* The change in voltage is scaled to a temperature coefficient of 10.0
   	* mV/degC (typical) for the MCP9700/9700A and 19.5 mV/degC (typical)
   	* for the MCP9701/9701A. The out- put voltage at 0 degC is also scaled
   	* to 500 mV (typical) and 400 mV (typical) for the MCP9700/9700A and
   	* MCP9701/9701A, respectively. VOUT = TC•TA+V0degC
   	*/

int temperature(TemperatureUnit unit) {
    int value = ((brain.temperatureSensor.getValue()*3.3)-400)/19.5;
    if (unit == TemperatureUnit::Celsius) return value;
    else return (9.0 / 5.0) * value + 32.0;
}

int lightLevel() {
    // 0...1023
    int value = brain.lightSensor.getValue();
    return value / 4;
}
