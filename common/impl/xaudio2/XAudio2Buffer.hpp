#pragma once

#ifdef X2AUDIO_SOUND

#include "../SoundBuffer.hpp"

class SoundEngine;

class XAudio2Buffer final : public SoundBuffer
{
public:
    XAudio2Buffer(const std::string& fileName, SoundEngine* engine);
    
private:
    bool LoadMemory() override { return true; }
    bool IsActive() const override { return true; }
};

#endif

