#ifndef BLUETOOTHCOMMUNICATOR_H
#define BLUETOOTHCOMMUNICATOR_H 

#include <Arduino.h>
#include <ArduinoQueue.h>

#define WAITING_LIST_SIZE 10

class BluetoothCommunicator {
private:
	ArduinoQueue<String> incoming = ArduinoQueue<String>(WAITING_LIST_SIZE);
	ArduinoQueue<String> outgoing = ArduinoQueue<String>(WAITING_LIST_SIZE);	

	Stream* serial;
	String lineEnding = "\r\n";
	
	String lastMessage;

	String partial = "";
	void processString(String string);
	void (*processMessage)(String);
	void processMessages();
	void send();
public:
	bool* btSwitch;
	void process();

	void receive();
	void send(String);
	BluetoothCommunicator(Stream*);
	void setMessageProcesser(void(*)(String));
	
	void setLineEnding(String);
	void repeatLastMessage();
};
#endif