#ifndef BRAINPAD_PIN_H
#define BRAINPAD_PIN_H

#include "MbedPin.h"
#include "mbed.h"

namespace codal {

    class BrainPadPin : public codal::_mbed::Pin {

	protected:

		void *pin;

	public:


		virtual int getAnalogValue();

		BrainPadPin(int id, PinNumber name, PinCapability capability);
	};

}

#endif