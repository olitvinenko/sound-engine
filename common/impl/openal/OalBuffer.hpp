#pragma once

#ifdef SOUND_OPENAL

#include "../SoundBuffer.hpp"

#include <OpenAL/OpenAL.h>


#include "OalSound.hpp"

struct InternalEngine;

class OalBuffer final : public SoundBuffer<OalSound>
{
public:
    OalBuffer(const std::string& fileName, InternalEngine* engine);
    ~OalBuffer();

    bool LoadMemory() override { return LoadBuffer(); }
    
    void AttachSource(OalSound* sound) override;
    void DetachSource(OalSound* sound) override;
    
private:
    bool IsActive() const override { return m_bufferID; }
    bool LoadBuffer();

private:
    ALuint m_bufferID = 0;
};

#endif
