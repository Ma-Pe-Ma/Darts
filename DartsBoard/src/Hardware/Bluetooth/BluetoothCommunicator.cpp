#include "BluetoothCommunicator.h"

BluetoothCommunicator::BluetoothCommunicator(Stream* serial) {
	this->serial = serial;

	for (int i = 0; i < MESSAGE_RECEIVER_NR; i++) {
		messageReceivers[i] = nullptr;
	}
}

void BluetoothCommunicator::process() {
	if (*btSwitch) {
		receive();
		send();
	}
}

void BluetoothCommunicator::send() {
	while (outgoing.itemCount() > 0) {
		String outgoingStr = outgoing.dequeue();
		serial->print(outgoingStr);
	}
}

void BluetoothCommunicator::receive() {
	while(serial->available() > 0) {
		String temp = serial->readString();

		if (btSwitch) {
			processString(temp);
		}
	}
	processMessages();
}

void BluetoothCommunicator::send(String message) {
	if (*btSwitch) {
		outgoing.enqueue(message + lineEnding);
		lastMessage = message;
	}
}

void BluetoothCommunicator::processString(String input) {
	bool wasBroken = false;
	String processable = partial + input;

	//Serial.println("Y: "+ processable +", size: "+processable.length());
	
	int condition =((int) processable.length()) - lineEnding.length() + 1;

	for (int i = 0; i < condition; i++) {
		if (processable.substring(i, i + lineEnding.length()) == lineEnding) {
			//Serial.println("Y0");
			String newEntry = processable.substring(0, i);
			//Serial.println("Y1");
			incoming.enqueue(newEntry);			

			String newProcessable = processable.substring(i + lineEnding.length());
			//Serial.println("Y2");

			partial = "";
			processString(newProcessable);
			//Serial.println("Y3");

			wasBroken = true;
			break;
		}
	}
		
	//Serial.println("Y5");

	if (!wasBroken) {
		partial = processable;
	}
}

void BluetoothCommunicator::processMessages() {	
	while (incoming.itemCount() > 0) {
		String incomingStr = incoming.dequeue();

		for (int i = 0; i < MESSAGE_RECEIVER_NR; i++) {
			if (messageReceivers[i] != nullptr) {
				messageReceivers[i]->receiveMessage(incomingStr);
			}
		}
	}
}

void BluetoothCommunicator::setLineEnding(String lineEnding) {
	this->lineEnding = lineEnding;
}

void BluetoothCommunicator::repeatLastMessage() {
	serial->print(lastMessage);
}

void BluetoothCommunicator::subscribeToMessages(MessageReceiver* messageReciver) {
	for (int i = 0; i < MESSAGE_RECEIVER_NR; i++) {
		if (messageReceivers[i] == nullptr) {
			messageReceivers[i] = messageReciver;
			break;
		}
	}
}

void BluetoothCommunicator::unsubscribeToMessages(MessageReceiver* messageReceiver) {
	for (int i = 0; i < MESSAGE_RECEIVER_NR; i++) {
		if (messageReceivers[i] == messageReceiver) {
			messageReceivers[i] = nullptr;
		}
	}
}