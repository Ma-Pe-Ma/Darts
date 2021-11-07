#include "DisplayContainer.h"
#include "ImageLoading.h"

DisplayContainer DisplayContainer::displayContainer;

void DisplayContainer::Initialize() {
	uint16_t ID;
    Serial.print("Show BMP files on DisplayContainer::displayContainer.getTFT() with ID:0x");
    ID = DisplayContainer::displayContainer.getTFT()->readID();
    Serial.println(ID, HEX);
    if (ID == 0x0D3D3) ID = 0x9481;
    displayContainer.getTFT()->begin(ID);
    displayContainer.getTFT()->fillScreen(0x07FF);
    displayContainer.getTFT()->setTextColor(0xFFFF, 0x0000);

	displayContainer.getTFT()->setRotation(1);
	displayContainer.setSizes(SCR_WIDTH, SCR_HEIGHT);
	displayContainer.setPressures(MINPRESSURE, MAXPRESSURE);
	displayContainer.setTouchScreenPins(XP, YP, XM, YM, RX);

	int cornerValues[4][2]={
		{TL_X_V, TL_Y_V},
		{TR_X_V, TR_Y_V},
		{BL_X_V, BL_Y_V},
		{BR_X_V, BR_Y_V}
	};	
	
	displayContainer.setTouchCornerValues(cornerValues);

	ImageLoading::initialize();
}

void DisplayContainer::setSizes(int width, int height) {
	this->width = width;
	this->height = height;
}

//first: 0-TL, 1-TR, 2-BL, 3-BR, second: 0-X, 1-Y
void DisplayContainer::setTouchCornerValues(int corner[4][2]) {
	A = ((corner[0][0] + corner[1][0]) + (corner[2][0] + corner[3][0]));
	B = ((corner[1][0] + corner[3][0]) - (corner[0][0] + corner[2][0]));
	C = ((corner[0][0] + corner[1][0]) - (corner[2][0] + corner[3][0]));
	D = ((corner[1][0] + corner[2][0]) - (corner[0][0] + corner[3][0]));

	E = ((corner[0][1] + corner[1][1]) + (corner[2][1] + corner[3][1]));
	F = ((corner[1][1] + corner[3][1]) - (corner[0][1] + corner[2][1]));
	G = ((corner[0][1] + corner[1][1]) - (corner[2][1] + corner[3][1]));
	H = ((corner[1][1] + corner[2][1]) - (corner[0][1] + corner[3][1]));
}

void DisplayContainer::setTouchScreenPins(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx) {
	this->xp = xp;
	this->yp = yp;
	this->xm = xm;
	this->ym = ym;
	this->rx = rx;
	
	touchScreen = TouchScreen(xp, yp, xm, ym, rx);
}

void DisplayContainer::setPressures(int minPressure, int maxPressure) {
	this->minPressure = minPressure;
	this->maxPressure = maxPressure;
}

Pair DisplayContainer::getTouchedPoint(){
	TSPoint p = touchScreen.getPoint();
    pinMode(yp, OUTPUT);      //restore shared pins
    pinMode(xm, OUTPUT);
    digitalWrite(yp, HIGH);   //because TFT control pins
    digitalWrite(xm, HIGH);
    bool pressed = (p.z > minPressure && p.z < maxPressure);
    
	int calX = -2;
	int calY = -2;
	
	Pair pair;
	
	if (pressed) {
		//Serial.println("Pressure: "+String(p.z));
		//Serial.println("RAW "+String(p.x)+", "+String(p.y));
		getCalibratedValue(calX, calY, p.y, p.x);
		pair.pressed = true;
    }
	
	pair.x = calX;
	pair.y = calY;
	
	return pair;
}

void DisplayContainer::getCalibratedValue(int& calX, int& calY, int rawX, int rawY) {
	/*Serial.println("A: "+String(A));
	Serial.println("B: "+String(B));
	Serial.println("C: "+String(C));
	Serial.println("D: "+String(D));
	
	Serial.println("E: "+String(E));
	Serial.println("F: "+String(F));
	Serial.println("G: "+String(G));
	Serial.println("H: "+String(H));*/

	int A_ = A - 4 * rawX;
	int E_ = E - 4 * rawY;

	long c2 = G * D - H * C;
	long c1 = B * G - F * C + E_ * D - H * A_;
	long c0 = E_ * B - F * A_;

	float eta = -2;
	float kszi = -2;
	
	/*Serial.println("c2: "+String(c2));
	Serial.println("c1: "+String(c1));
	Serial.println("c0: "+String(c0));*/

	if (c2 == 0) {

		if (c1 == 0) {
			Serial.println("Equation can't be solved");
		}
		else {
			eta =  - c1 / c0; 
		}
	}
	else{

		long abs_c2 = abs(c2);

		int determinant = (c1/c2)*(c1/c2) - 4*(c2/c2)*(c0/c2);

		if (determinant < 0) {
			Serial.println("Determinant is negative: "+String(determinant));
		}
		else {
			//TODO: find proper value, not by comparing!
			eta = (- (c1) / (2 * c2)) - (c2 / abs_c2 ) * sqrt(determinant)/2;
			
			if (abs(eta) > 1.1) {
				eta = (- (c1) / (2 * c2)) + (c2 / abs_c2 ) * sqrt(determinant)/2;
			}
		}
	}

	if (eta != -2) {

		float divider = B+D*eta;

		if (divider == 0) {
			Serial.println("Continuity error!");
		}
		else {
			kszi = - (C * eta + A_) /(D * eta + B);
		}

	}

	if (kszi != -2 && eta != -2) {
		calX = (int) (width / 2) * (kszi + 1);
		calY = (int) (height / 2) * (1 - eta);
		//Serial.println("eta: "+String(eta));
		//Serial.println("kszi "+String(kszi));
		
		//Serial.println("Calibrated x: "+String(calX)+" ("+String(rawY)+")");
		//Serial.println("Calibrated y: "+String(calY)+" ("+String(rawX)+")");
	}
}


void DisplayContainer::Write(int x, int y, int color, int size, String text) {
	tft.setCursor(x, y);
	tft.setTextColor(color);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::WriteRight(int a, int b, int color, int size,  String text) {	
	int x = (SCR_WIDTH - text.length() * 6 * size - a);
	int y = b;
	
	tft.setCursor(x, y);
	tft.setTextColor(color);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::WriteWithBackground(int x, int y, int color, int back, int size, String text) {	
	tft.setCursor(x, y);
	tft.setTextColor(color, back);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::WriteCenterX(int y, int color, int backColor, int size, String text) {
	int x = (SCR_WIDTH - text.length() * size * 6) / 2;

	tft.setCursor(x, y);
	tft.setTextColor(color, backColor);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::WriteCenterY(int x, int color, int backColor, int size, String text) {
	int y = (SCR_HEIGHT - size * 6) / 2;

	tft.setCursor(x, y);
	tft.setTextColor(color, backColor);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::WriteCenter(String text, int size) {
	int x = (SCR_WIDTH - text.length() * 6 * size) / 2;
	int y = (SCR_HEIGHT - 6 * size) / 2;
	
	tft.setCursor(x, y);
	tft.setTextColor(RED, GREEN);
	tft.setTextSize(size);
	tft.print(text);
}

String DisplayContainer::SectorText(Sector sector) {	
	String m;
	if (sector.multiplier == 0) {
		m = "-  ";
	}		
	else {
		if (sector.multiplier == 1)  {
			m = ' ';
		}			
		if (sector.multiplier == 2) {
			m = 'D';
		}			
		if (sector.multiplier == 3) {
			m = 'T';
		}			
		
		if (sector.base < 10) {
			m = m + String(sector.base) + ' ';
		}			
		else {
			m = m + String(sector.base);
		}			
	}
	
	return m;
}