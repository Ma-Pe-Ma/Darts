#ifndef SIMPLEMAP_H
#define SIMPLEMAP_H

#include "Arduino.h"

template<typename T, typename U>
class SimpleMap {
	int currentSize = 0;
	int maxSize;
	
	T* key;
	U* value;
	
public:
	SimpleMap(int maxSize) {
		this->maxSize = maxSize;
	
		key = new T[maxSize];
		value = new U[maxSize];
	}
	
	bool insert(T e1, U e2) {
		if (currentSize < maxSize) {
			key[currentSize] = e1;	
			value[currentSize] = e2;		
		
			currentSize++;
			return true;
		}

		return false;
	}
	
	U getValueByKey(T key) {
		for (int i = 0; i < currentSize; i++) {
			if (key == this->key[i] ) {
				return this->value[i];
			}
		}
		
		Serial.println("Key not found: " + String(key));
	}

	T getKeyByValue(U value) {
		for (int i = 0; i < currentSize; i++) {
			if (value == this->value[i] ) {
				return this->key[i];
			}
		}
	}
	
	~SimpleMap() {
		delete[] key;
		delete[] value;
	}
};
#endif