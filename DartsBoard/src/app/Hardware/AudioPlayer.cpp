#include "AudioPlayer.h"
#include <Audio.h>
#include <SD.h>

namespace AudioPlayer {
    void initialize() {
        Audio.begin(88200, 100);
        Serial.println("Audio initialized...");
    }

    void play(Resources::AudioFile audioFile) {
        //String fileName = "/res/audio/";
        char* fileName = Resources::getAudioFileByID(audioFile);
        Serial.print("Playing file: ");
        Serial.println(fileName);
        delay(200);

        play(fileName);
    }

    void play(char* filename) {
        File myFile = SD.open(filename);
        
        if (!myFile) {
            Serial.print("Error opening: ");
            Serial.println(filename);
            return;
        }

        const int S = 1024; // Number of samples to read in block
        short buffer[S];

        Serial.print("Playing file: ");
        Serial.println(filename);
 
        while (myFile.available()) {
            myFile.read(buffer, sizeof(buffer));

            // Prepare samples
            int volume = 1024;

            Audio.prepare(buffer, S, volume);
            // Feed samples to audio

            Audio.write(buffer, S);
        }

        myFile.close();
    }   
}