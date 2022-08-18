#include "DisplayContainer.h"

DisplayContainer::DisplayContainer() {
   
}

void DisplayContainer::init() {
	Serial.print("Show BMP files on tft with ID:0x");
    uint16_t ID = tft.readID();
    Serial.println(ID, HEX);
    if (ID == 0x0D3D3) ID = 0x9481;
    tft.begin(ID);
    tft.fillScreen(0x07FF);
    tft.setTextColor(0xFFFF, 0x0000);

	tft.setRotation(1);
	setSizes(SCR_WIDTH, SCR_HEIGHT);
	setPressures(MINPRESSURE, MAXPRESSURE);
	setTouchScreenPins(XP, YP, XM, YM, RX);

	int cornerValues[4][2]={
		{TL_X_V, TL_Y_V},
		{TR_X_V, TR_Y_V},
		{BL_X_V, BL_Y_V},
		{BR_X_V, BR_Y_V}
	};	
	
	setTouchCornerValues(cornerValues);

	bool good = SD.begin(SD_CS);
	if (!good) {
		Serial.print(F("cannot start SD"));
		while (1);
	}
        
	root = SD.open(namebuf);
	pathlen = strlen(namebuf);
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
    
	//values which passed as reference
	int calX = -2;
	int calY = -2;
	
	Pair pair;
	
	if (pressed) {
		//Serial.println("Pressure: "+String(p.z));
		getCalibratedValue(calX, calY, p.y, p.x);
		//Serial.println("RAW "+String(p.x)+", "+String(p.y) + "  -  CAL "+String(calX)+", "+String(calY));
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


void DisplayContainer::write(int x, int y, int color, int size, String text) {
	tft.setCursor(x, y);
	tft.setTextColor(color);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::writeRight(int a, int b, int color, int size,  String text) {	
	int x = (SCR_WIDTH - text.length() * 6 * size - a);
	int y = b;
	
	tft.setCursor(x, y);
	tft.setTextColor(color);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::writeWithBackground(int x, int y, int color, int back, int size, String text) {	
	tft.setCursor(x, y);
	tft.setTextColor(color, back);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::writeCenterX(int y, int color, int backColor, int size, String text) {
	int x = (SCR_WIDTH - text.length() * size * 6) / 2;

	tft.setCursor(x, y);
	tft.setTextColor(color, backColor);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::writeCenterY(int x, int color, int backColor, int size, String text) {
	int y = (SCR_HEIGHT - size * 6) / 2;

	tft.setCursor(x, y);
	tft.setTextColor(color, backColor);
	tft.setTextSize(size);
	tft.print(text);
}

void DisplayContainer::writeCenter(String text, int size) {
	int x = (SCR_WIDTH - text.length() * 6 * size) / 2;
	int y = (SCR_HEIGHT - 6 * size) / 2;
	
	tft.setCursor(x, y);
	tft.setTextColor(RED, GREEN);
	tft.setTextSize(size);
	tft.print(text);
}

String DisplayContainer::sectorText(Sector sector) {	
	String m = "-";
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

uint8_t DisplayContainer::showBMP(char *nm, int x, int y) {
	Serial.print("Showing BMP...");
	File bmpFile;
	int bmpWidth, bmpHeight;    // W+H in pixels
	uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
	uint32_t bmpImageoffset;    // Start of image data in file
	uint32_t rowSize;           // Not always = bmpWidth; may have padding
	uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
	uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
	uint8_t bitmask, bitshift;
	boolean flip = true;        // BMP is stored bottom-to-top
	int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
	uint32_t pos;               // seek position
	boolean is565 = false;      //

	uint16_t bmpID;
	uint16_t n;                 // blocks read
	uint8_t ret;

	if ((x >= tft.width()) || (y >= tft.height()))
		return 1;               // off screen

	bmpFile = SD.open(nm);      // Parse BMP header
	bmpID = read16(bmpFile);    // BMP signature
	(void) read32(bmpFile);     // Read & ignore file size
	(void) read32(bmpFile);     // Read & ignore creator bytes
	bmpImageoffset = read32(bmpFile);       // Start of image data
	(void) read32(bmpFile);     // Read & ignore DIB header size
	bmpWidth = read32(bmpFile);
	bmpHeight = read32(bmpFile);
	n = read16(bmpFile);        // # planes -- must be '1'
	bmpDepth = read16(bmpFile); // bits per pixel
	pos = read32(bmpFile);      // format
	if (bmpID != 0x4D42) ret = 2; // bad ID
	else if (n != 1) ret = 3;   // too many planes
	else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
	else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette 
	else {
		bool first = true;
		is565 = (pos == 3);               // ?already in 16-bit format
		// BMP rows are padded (if needed) to 4-byte boundary
		rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
		if (bmpHeight < 0) {              // If negative, image is in top-down order.
			bmpHeight = -bmpHeight;
			flip = false;
		}

		w = bmpWidth;
		h = bmpHeight;
		if ((x + w) >= tft.width())       // Crop area to be loaded
			w = tft.width() - x;
		if ((y + h) >= tft.height())      //
			h = tft.height() - y;

		if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
			bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
			bitmask = 0xFF;
			if (bmpDepth < 8)
				bitmask >>= bmpDepth;
			bitshift = 8 - bmpDepth;
			n = 1 << bmpDepth;
			lcdbufsiz -= n;
			palette = lcdbuffer + lcdbufsiz;
			for (col = 0; col < n; col++) {
				pos = read32(bmpFile);    //map palette to 5-6-5
				palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
			}
		}

		// Set tft address window to clipped image bounds
		tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
		for (row = 0; row < h; row++) { // For each scanline...
			// Seek to start of scan line.  It might seem labor-
			// intensive to be doing this on every line, but this
			// method covers a lot of gritty details like cropping
			// and scanline padding.  Also, the seek only takes
			// place if the file position actually needs to change
			// (avoids a lot of cluster math in SD library).
			uint8_t r, g, b, *sdptr;
			int lcdidx, lcdleft;
			if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
				pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
			else        // Bitmap is stored top-to-bottom
				pos = bmpImageoffset + row * rowSize;
			if (bmpFile.position() != pos) { // Need seek?
				bmpFile.seek(pos);
				buffidx = sizeof(sdbuffer); // Force buffer reload
			}

			for (col = 0; col < w; ) {  //pixels in row
				lcdleft = w - col;
				if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
				for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
					uint16_t color;
					// Time to read more pixel data?
					if (buffidx >= sizeof(sdbuffer)) { // Indeed
						bmpFile.read(sdbuffer, sizeof(sdbuffer));
						buffidx = 0; // Set index to beginning
						r = 0;
					}
					switch (bmpDepth) {          // Convert pixel from BMP to tft format
						case 24:
							b = sdbuffer[buffidx++];
							g = sdbuffer[buffidx++];
							r = sdbuffer[buffidx++];
							color = tft.color565(r, g, b);
							break;
						case 16:
							b = sdbuffer[buffidx++];
							r = sdbuffer[buffidx++];
							if (is565)
								color = (r << 8) | (b);
							else
								color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
							break;
						case 1:
						case 4:
						case 8:
							if (r == 0)
								b = sdbuffer[buffidx++], r = 8;
							color = palette[(b >> bitshift) & bitmask];
							r -= bmpDepth;
							b <<= bmpDepth;
							break;
					}
					lcdbuffer[lcdidx] = color;

				}
				tft.pushColors(lcdbuffer, lcdidx, first);
				first = false;
				col += lcdidx;
			}           // end cols
		}               // end rows
		tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
		ret = 0;        // good render
	}
	bmpFile.close();
	//Serial.println("width: " + String(tft.width()) +", "+String(tft.height()));
	return (ret);
}

uint16_t DisplayContainer::read16(File& f) {
	uint16_t result;         // read little-endian
	f.read(&result, sizeof(result));
	return result;
}

uint32_t DisplayContainer::read32(File& f) {
	uint32_t result;
	f.read(&result, sizeof(result));
	return result;
}