#pragma once

#include "../../include/Sound.hpp"

#include <OpenAL/OpenAL.h>
#include <memory>

class OalBuffer;

class OalSound : public Sound
{
    friend class OalSoundEngine;
public:
    OalSound(std::shared_ptr<OalBuffer> buffer, bool isAutoDelete);
    ~OalSound();
    
protected:
    void Play() override;
    void Pause() override;
    void Stop() override;
    
    void SetLoop(bool isLoop) override;
    bool IsLoopped() const override;
    
    void SetTime(float timeSeconds) override;
    float GetTime() const override;
    
    bool IsPlaying() const override;
    bool IsPaused() const override;
    bool IsStopped() const override;
    
    void SetVolume(float volume) override;
    float GetVolume() const override { return m_volume; }
    
    float GetDurationSec() const override { return m_duration; }
    
public:
    void UnloadBuffer();
    const std::string& GetFileName() const;
    
private:
    bool Volume(float volume);
    
private:
    ALfloat m_currentTime;
    ALfloat m_saveCurrentTime;
    
    bool m_isLoop;
    float m_volume;
    
    float m_duration;
    bool m_isAutoDelete;
    
    ALuint m_sourceID;
    std::shared_ptr<OalBuffer> m_buffer;
};
