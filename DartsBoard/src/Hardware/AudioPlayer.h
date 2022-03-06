#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "Arduino.h"
#include "../Resource/Resources.h"

namespace AudioPlayer {
    void initialize();
    void play(Resources::AudioFile);
    void play(String);
}

#endif