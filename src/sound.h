#ifndef SOUND_H
#define SOUND_H

#include <cstdint>
#include <cmath>
#include <SDL.h>

class Sound
{
    public:
        Sound();
        virtual ~Sound();
        bool initSoundGenerator(void);
        void playSound(void);
};

#endif // SOUND_H
