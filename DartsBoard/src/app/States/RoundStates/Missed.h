#ifndef MISSED_H
#define MISSED_H

#include "Hit.h"

class Missed : public Hit {
public:
    Missed(GamePlayingScreen* gamePlayingScreen) : Hit(gamePlayingScreen) {}
    void update(Pair) override;

    void initialize() override {
        //audioFileName = Resources::getAudioFileByID(Resources::AudioFile::pathetic);
    }
};

#endif