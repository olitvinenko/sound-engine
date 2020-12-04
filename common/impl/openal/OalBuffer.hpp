#pragma once

#include <OpenAL/OpenAL.h>

#include "../SoundBuffer.hpp"

class OalSoundEngine;

class OalBuffer final : public SoundBuffer
{
public:
    OalBuffer(const std::string& fileName, OalSoundEngine* engine);
    ~OalBuffer();

    bool LoadMemory() override { return LoadBuffer(); }
    
    void AttachSource(Sound* sound) override;
    void DetachSource(Sound* sound) override;
    
private:
    bool IsActive() const override { return m_bufferID; }
    bool LoadBuffer();

private:
    ALuint m_bufferID = 0;
};
