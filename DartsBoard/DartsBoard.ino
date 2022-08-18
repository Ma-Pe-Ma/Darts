#include "src/Hardware/Bluetooth/BluetoothCommunicator.h"

#include "src/States/GameLogic.h"

#include "src/Hardware/DisplayContainer.h"
#include "src/Player/PlayerContainer.h"
#include "src/Games/GameContainer.h"

#include "src/Resource/Resources.h"

#include "src/Hardware/AudioPlayer.h"

/*//#include <soft_uart.h>
#define RX_PIN 22 // software serial port's reception pin
#define TX_PIN 24 // software serial port's transmision pin
#define SOFT_UART_BIT_RATE 9600 // 57600 38400 1200 19200 9600 115200 300
#define RX_BUF_LENGTH 64 // software serial port's reception buffer length
#define TX_BUF_LENGTH 64 // software serial port's transmision buffer length

serial_tc4_declaration(RX_BUF_LENGTH,TX_BUF_LENGTH);
auto& serial_obj = serial_tc4;*/

BluetoothCommunicator bluetooth(&Serial1);

DisplayContainer displayContainer;
PlayerContainer playerContainer;
GameContainer gameContainer(&displayContainer, &playerContainer);

GameLogic gameLogic(&displayContainer, &playerContainer, &gameContainer);

void setup() {
	Serial.begin(115200);
	Serial1.begin(9600);

	Serial.println("Setup started!");
	
	Resources::textSet = Resources::en;
	Resources::audioSet = Resources::mk;

	//Setting piezo speaker output
	//pinMode(22, OUTPUT);

	displayContainer.init();
	Resources::initialize();
	playerContainer.init();
	gameContainer.init();
	gameLogic.init();
	
	bluetooth.btSwitch = &gameLogic.androidMode;

	//config bluetoothsettings
	bluetooth.subscribeToMessages(&gameLogic);
	gameLogic.bluetoothCommunicator = &bluetooth;

	/*serial_obj.begin(
		RX_PIN,
		TX_PIN,
		SOFT_UART_BIT_RATE,
		soft_uart::data_bit_codes::EIGHT_BITS,
		soft_uart::parity_codes::NO_PARITY,
		soft_uart::stop_bit_codes::ONE_STOP_BIT
  	);*/

	AudioPlayer::initialize();
	
	gameLogic.transitionTo(&gameLogic.mainScreen);
	Serial.println("Setup finished!");
}

void loop() {	
	//bluetooth input
	bluetooth.process();

	//the main logic of the application
	gameLogic.run();
}