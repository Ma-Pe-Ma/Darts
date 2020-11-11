#ifndef BUTTON_H
#define BUTTON_H

#include "Pair.h"
#include <Adafruit_GFX.h>

class Button {
	private:				
		bool pushed = false;
		unsigned long pushStartTime;
		unsigned long lastDetectionTime;
		int debounceTime = 160;
		bool prevState = false;

		bool simplePushed = false;
		bool impulsePushed = false;
		bool continuousPushed = false;
		bool fasteningPushed = false;
		
		bool impulseChange;
		float impulseTime = 100;
		
		float fasteningTime;
		float fasteningRef;
		
		bool change = false;
		bool release = false;
		
	public:
		String label ="";
		Adafruit_GFX_Button guiButton;
	
		void detect(bool);
		void detect(Pair);
		void detectAlter(bool);
		bool simple();			
		bool impulse();			
		bool continuous();
		bool fastening();		

		void setDebounceTime(float);
		void setImpulseTime(float);
		
		bool isPushed();
		bool released();

		void setImage(Adafruit_GFX*, int16_t, int16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, char *, uint8_t);

		Button();
};

#endif