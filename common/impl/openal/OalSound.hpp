#pragma once

#include "../../include/Sound.hpp"

#include <OpenAL/OpenAL.h>
#include <memory>

class OalBuffer;

class OalSound
    : public Sound
    , public std::enable_shared_from_this<OalSound>
{
    friend class OalSoundEngine;
    
private:
    OalSound(OalBuffer* buffer, bool isAutoDelete);
    ~OalSound();
public:
    
public:
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
    
    void Delete() override;
    
public:
    const std::string& GetFileName() const;
    ALuint GetSourceId() const { return m_sourceID; }
    
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
    OalBuffer* m_buffer;
};
