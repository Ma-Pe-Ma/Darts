#ifndef BUSTED_H
#define BUSTED_H

#include "Hit.h"

class Busted : public Hit {
public:
    Busted(GamePlayingScreen* gamePlayingScreen) : Hit(gamePlayingScreen) {}

    void update(Pair) override;

    void initialize() override {
        audioFileName = Resources::getAudioFieByID(Resources::AudioFile::pathetic);
    }
};

#endif