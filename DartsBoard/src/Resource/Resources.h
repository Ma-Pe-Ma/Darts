#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "../SimpleMap.h"
#include "Arduino.h"

#define NUMBER_OF_TEXT_SETS 2
#define NUMBER_OF_TEXTS 35

#define NUMBER_OF_AUDIO_SETS 2
#define NUMBER_OF_AUDIO_FILES 6

#define STRING_RESOURCE_LENGTH 32

namespace Resources {
    extern String path;

    enum class TextSet {
        en,
        hu
    };

    extern TextSet textSet;
    
    enum Text {
        gameScheme,
        players,
        playersAndroid1,
        playersAndroid2,
        playerNr,
        settings,

        cricket,
        rtc,
        x01,
        config,

        rtcClassic,
        shootOut,

        cricketScore,
        cricketNoscore,
        cricketCutthroat,

        cricketClassic,
        cricketAll,
        cricketCustom,

        cricketInterval,
        cricketRandinterval,
        cricketChaotic,

        cricketNrOfNrs,
        cricketStartingNr,

        rtcSector,
        rtcMultiplier,
        rtcMultiplierWithPoints,

        x01Simple,
        x01Double,
        x01Master,
        
        x01In,
        x01Out,
        x01Score
    };

    extern SimpleMap<Text, char*> textMap;

    void initialize();
    void processLine(String line, String& key, String& value);

    enum class AudioSet {
        en,
        hu
    };

    extern AudioSet audioSet;
    extern SimpleMap<AudioSet, String> audioSetNameMap;

    //TODO: create audiofiles
    enum class AudioFile {
        start,
        round,
        pathetic
    };

    extern SimpleMap<AudioFile, char*> audioMap;
    char* getTextByID(Text text); 
    char* getAudioFileByID(AudioFile audioFile);
}

#endif