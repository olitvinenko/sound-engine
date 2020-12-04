#pragma once

#include "../Sound.hpp"

#include <OpenAL/OpenAL.h>
#include <memory>

class OalBuffer;

class OalSound final : public Sound
{
    friend class OalSoundEngine;
    
private:
    OalSound(OalBuffer* buffer, bool isAutoDelete);
    ~OalSound();

public:
    bool Play() override;
    bool Pause() override;
    bool Stop() override;
    
    bool SetLoop(bool isLoop) override;
    bool IsLoopped() const override;
    
    bool SetTime(float timeSeconds) override;
    float GetTime() const override;
    
    bool IsPlaying() const override;
    bool IsPaused() const override;
    bool IsStopped() const override;
    
    bool SetVolume(float volume) override;
    float GetVolume() const override { return m_volume; }
    
    float GetDurationSec() const override { return m_duration; }

public:
    ALuint GetSourceId() const { return m_sourceID; }

private:
    bool IsValid() const override { return m_sourceID != 0; }
    bool Volume(float volume);
private:
    ALuint m_sourceID;
};
