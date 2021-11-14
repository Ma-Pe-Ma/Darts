#include "Resources.h"
#include <SD.h>

namespace Resources {
    String path = "/res/";

    TextSet textSet = TextSet::en;

    SimpleMap<TextSet, String> textSetNameMap(NUMBER_OF_TEXT_SETS);
    
    SimpleMap<String, Text> textNameMap(NUMBER_OF_TEXTS);
    SimpleMap<Text, String> textMap(NUMBER_OF_TEXTS);

    AudioSet audioSet = AudioSet::normal;
    SimpleMap<AudioSet, String> audioSetNameMap(NUMBER_OF_AUDIO_SETS);
    SimpleMap<AudioFile, String> audioMap(NUMBER_OF_AUDIO_FILES);

    void readFile(File* myFile, void(*processKeyPair)(String, String)) {
        String line = "";
        String key = "";
        String value = "";

        while (myFile->available()) {
            char character = myFile->read();       

            bool process = false;
            if (character == -1) {
                process = true;
            }
            else {
                line.concat(character);
            }

            if (line.length() > 1) {
                if (character == '\n' && line.charAt(line.length() - 2) == '\r') {
                    process = true;
                }
            }

            if (process) {
                processLine(line.substring(0, line.length() - 2), key, value);

                if (!(key == "")) {
                    processKeyPair(key, value);
                }
                key = "";
                value = "";

                line = "";
            }
        }

        processLine(line, key, value);
        if (!(key == "")) {
            processKeyPair(key, value);
        }
    }

    void addText(String keyString, String value) {
        Serial.println("key: " + keyString + ", value: " + value);

        Text key = textNameMap.getValueByKey(keyString);
        textMap.insert(key, value);
    }

    void addAudio(AudioFile key, String value) {
        String fileLocation = path + "audio/" + audioSetNameMap.getValueByKey(audioSet) + "/" + value + "wav";
        audioMap.insert(key, fileLocation);
    }

    void printDirectory(File dir, int numTabs) {
        while (true) {
            File entry =  dir.openNextFile();

            if (! entry) {
                // no more files
                break;
            }

            for (uint8_t i = 0; i < numTabs; i++) {
                Serial.print('\t');
            }

            Serial.print(entry.name());

            if (entry.isDirectory()) {
                Serial.println("/");
                printDirectory(entry, numTabs + 1);
            } 
            else {
            // files have sizes, directories do not
                Serial.print("\t\t");
                Serial.println(entry.size(), DEC);
            }

            entry.close();
        }
    }

    void initialize() {
        textSetNameMap.insert(TextSet::en, String("en"));
        textSetNameMap.insert(TextSet::hu, String("hu"));

        //------WARNING------- File/DIR name cannot be longer than 8 chars!
        textNameMap.insert(String("cricket"), Text::cricket);
        textNameMap.insert(String("rtc"), Text::rtc);
        textNameMap.insert(String("x01"), Text::x01);
        textNameMap.insert(String("config"), Text::config);

        String textPath = path + "text/" + textSetNameMap.getValueByKey(textSet) + ".txt";
        File myFile = SD.open(textPath);
        readFile(&myFile, addText);

        /*File root = SD.open("/");
        printDirectory(root, 0);*/

        audioSetNameMap.insert(AudioSet::normal, String("normal"));
        audioSetNameMap.insert(AudioSet::mk, String("mk"));

        addAudio(AudioFile::pathetic, "pathetic");        
    }

    void processLine(String line, String& key, String& value) {
        for(int i = 0; i < line.length(); i++) {
            if (line.charAt(i) == ' ') {
                key = line.substring(0, i);
                value = line.substring(i + 1);
                break;
            }
        }
    }

    String getTextByID(Text text) {
        return textMap.getValueByKey(text);
    }

    String getAudioFieByID(AudioFile audioFile) {
        return audioMap.getValueByKey(audioFile);
    }
}