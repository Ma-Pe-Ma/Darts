#ifndef SIMPLEMAP_H
#define SIMPLEMAP_H

#include "Arduino.h"

template<typename T, typename U>
class SimpleMap {
	uint8_t currentSize = 0;
	uint8_t maxSize;
	
	T* key;
	U* value;
	
public:
	SimpleMap(uint8_t maxSize) {
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
		for (uint8_t i = 0; i < currentSize; i++) {
			if (key == this->key[i] ) {
				return this->value[i];
			}
		}
	}

	T getKeyByValue(U value) {
		for (uint8_t i = 0; i < currentSize; i++) {
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