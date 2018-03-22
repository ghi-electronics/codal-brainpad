#include "BrainPad.h"
#include "BrainPadDisplay.h"
#include <string>
#include "GraphicsHelper.h"

BrainPad brain;
GraphicsHelper graphics;

// Simple buttons test
void TestButtons(){
   if(brain.buttonUp.isPressed())
       graphics.DrawString(1, 40, "UP   ");
   if(brain.buttonLeft.isPressed())
       graphics.DrawString(1, 40, "LEFT ");
   if(brain.buttonRight.isPressed())
       graphics.DrawString(1, 40, "RIGHT");
   if(brain.buttonDown.isPressed())
       graphics.DrawString(1, 40, "DOWN ");
 }

void TestBuzzer(){
    // Turn the buzzer on at 1800hz for 300ms
    int frequency = 1800;
    brain.io.buzzer.setAnalogValue(512); //per https://github.com/Microsoft/pxt-brainpad/blob/master/libs/music/music.cpp#L48
    brain.io.buzzer.setAnalogPeriodUs(1000000 / frequency);
    brain.sleep(300);
    brain.io.buzzer.setAnalogValue(0);
}

void TestLightBulb(){
    // Slowley turn the LED on
    for(int i=0; i<10; i++)	{
    brain.io.ledBlue.setAnalogValue(i * 100);
    brain.sleep(200);
    }
    // Turn it off
    brain.io.ledBlue.setAnalogValue(0);
}

// show the light level on the screen
void TestLightSensor(){
    // see: https://github.com/Microsoft/pxt-common-packages/blob/master/libs/lightsensor/lightsensor.cpp#L48
    int light = brain.io.lightPin.getAnalogValue() / 16; // returned values are 16K max, change to 1K max.
    std::string l = "L:" + std::to_string(light) + "  ";
    graphics.DrawString(1, 20, l);
    // writeScreenBuffer is in the main loop
}


// show temp in Celisius
void TestTemperatureSensor(){
    int temper = (((brain.io.temperaturePin.getAnalogValue() / 16383.0) * 3300) - 450) / 19.5; // normalize to 10 bits output
    std::string t = "T:" + std::to_string(temper) + "  ";
    graphics.DrawString(1, 1, t);
    // writeScreenBuffer is in the main loop
}

// requires servo motor on servo pins #1
void TestServo() {
    brain.io.servoOne.setServoValue(180);
    brain.sleep(200);
    brain.io.servoOne.setServoValue(90);
    brain.sleep(200);
    brain.io.servoOne.setServoValue(0);
    brain.sleep(200);
}

// Event test
// This doesn't work yet.. investigating
/*void onButton(Event)
{
    brain.io.ledRed.setDigitalValue(1);
}*/

int main() {
    brain.init();
    TestServo();
    TestBuzzer();
    TestLightBulb();
    // Events are being looked at still
    //brain.messageBus.listen(ID_PIN_BUTTON_LEFT, DEVICE_BUTTON_EVT_CLICK, onButton);
        
    while (true) {
        TestButtons();
        TestTemperatureSensor();
        TestLightSensor();
        // show the messages on the screen
        brain.lcd.writeScreenBuffer(graphics.vram); 
    }
   return 0;
}
