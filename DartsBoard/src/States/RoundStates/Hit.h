#ifndef HIT_H
#define HIT_H

#include "../RoundState.h"

#include "../../Resource/Resources.h"

class Hit : public RoundState {
protected:
    int dartStatusStartX = SCR_WIDTH / 10;
    int dartStatusOffsetX = SCR_WIDTH / 3;

    int dartStatusStartY = SCR_HEIGHT / 2;

    String audioFileName;

public:
    virtual void start() override;
    virtual void update(Pair) override;

    Hit(GamePlayingScreen* gamePlayingScreen) : RoundState(gamePlayingScreen) {}

    void initialize() {
        //audioFileName = Resources::getAudioFileByID(Resources::AudioFile::pathetic);
    }
};

#endif