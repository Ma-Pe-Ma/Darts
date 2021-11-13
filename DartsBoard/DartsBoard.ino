// MCUFRIEND UNO shields have microSD on pins 10, 11, 12, 13
// The official <SD.h> library only works on the hardware SPI pins
// e.g. 11, 12, 13 on a Uno
// e.g. 50, 51, 52 on a Mega2560
// e.g. 74, 75, 76 on a Due
//
// if you are not using a UNO,  you must use Software SPI:
//
// install v1.0.1 of the <SdFat.h> library with the Arduino Library Manager.
// edit the src/SdFatConfig.h file to #define ENABLE_SOFTWARE_SPI_CLASS 1
//
// copy all your BMP files to the root directory on the microSD with your PC
// (or another directory)

//#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <soft_uart.h>

#include "src/Hardware/Bluetooth/BluetoothCommunicator.h"

#include "src/States/GameLogic.h"

#include "src/Hardware/DisplayContainer.h"
#include "src/Player/PlayerContainer.h"
#include "src/Games/GameContainer.h"

#define RX_PIN 22 // software serial port's reception pin
#define TX_PIN 24 // software serial port's transmision pin
#define SOFT_UART_BIT_RATE 9600 // 57600 38400 1200 19200 9600 115200 300
#define RX_BUF_LENGTH 64 // software serial port's reception buffer length
#define TX_BUF_LENGTH 64 // software serial port's transmision buffer length

serial_tc4_declaration(RX_BUF_LENGTH,TX_BUF_LENGTH);
auto& serial_obj = serial_tc4;

BluetoothCommunicator bluetooth(&serial_obj);

DisplayContainer displayContainer;
PlayerContainer playerContainer;
GameContainer gameContainer(&displayContainer);

GameLogic gameLogic(&displayContainer, &playerContainer, &gameContainer);

void setup() {
	Serial.begin(115200);
	Serial.println("ST STARTED");
	
	//Setting piezo speaker output
	pinMode(22, OUTPUT);

	displayContainer.init();
	playerContainer.init();
	gameContainer.init();
	gameLogic.init();
	
	bluetooth.btSwitch = &gameLogic.androidMode;
	Serial.println("ST Finished1");

	//config bluetoothsettings
	bluetooth.subscribeToMessages(&gameLogic);
	Serial.println("ST Finished2");
	gameLogic.bluetoothCommunicator = &bluetooth;

	Serial.println("ST Finished3");

	serial_obj.begin(
		RX_PIN,
		TX_PIN,
		SOFT_UART_BIT_RATE,
		soft_uart::data_bit_codes::EIGHT_BITS,
		soft_uart::parity_codes::NO_PARITY,
		soft_uart::stop_bit_codes::ONE_STOP_BIT
  	);
	
	gameLogic.transitionTo(&gameLogic.mainScreen);
	Serial.println("ST Finished");
}

void loop() {	
	//bluetooth input
	bluetooth.process();

	//the main logic of the application
	gameLogic.run();	
}