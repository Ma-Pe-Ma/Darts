#ifndef BLUETOOTHCOMMUNICATOR_H
#define BLUETOOTHCOMMUNICATOR_H 

#include <Arduino.h>
#include <ArduinoQueue.h>

#define WAITING_LIST_SIZE 10
#define MESSAGE_RECEIVER_NR 5

#include "MessageReceiver.h"

class BluetoothCommunicator {
private:
	ArduinoQueue<String> incoming = ArduinoQueue<String>(WAITING_LIST_SIZE);
	ArduinoQueue<String> outgoing = ArduinoQueue<String>(WAITING_LIST_SIZE);	

	Stream* serial;
	String lineEnding = "\r\n";
	
	String lastMessage;

	String partial = "";
	void processString(String string);
	void processMessages();
	void send();

	MessageReceiver** messageReceivers = new MessageReceiver*[MESSAGE_RECEIVER_NR];
public:
	bool* btSwitch;
	void process();

	void receive();
	void send(String);
	BluetoothCommunicator(Stream*);
	
	void setLineEnding(String);
	void repeatLastMessage();

	void subscribeToMessages(MessageReceiver*);
	void unsubscribeToMessages(MessageReceiver*);
};
#endif