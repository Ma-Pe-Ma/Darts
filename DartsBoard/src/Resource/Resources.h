#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "../SimpleMap.h"
#include "Arduino.h"

#define NUMBER_OF_TEXT_SETS 2
#define NUMBER_OF_TEXTS 10

#define NUMBER_OF_AUDIO_SETS 2
#define NUMBER_OF_AUDIO_FILES 6

namespace Resources {
    extern String path;

    enum TextSet {
        en,
        hu
    };

    extern TextSet textSet;
    extern SimpleMap<TextSet, String> textSetNameMap;
    
    enum Text {
        cricket,
        rtc,
        x01,
        config
    };

    extern SimpleMap<String, Text> textNameMap;
    extern SimpleMap<Text, String> textMap;

    void initialize();
    void processLine(String line, String& key, String& value);

    enum AudioSet {
        normal,
        mk
    };

    extern AudioSet audioSet;
    extern SimpleMap<AudioSet, String> audioSetNameMap;

    enum AudioFile {
        start,
        round,
        pathetic
    };

    extern SimpleMap<AudioFile, String> audioMap;
    String getTextByID(Text text); 
    String getAudioFieByID(AudioFile audioFile);
}

#endif