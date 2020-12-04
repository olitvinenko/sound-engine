#pragma once

#include <string>
#include <memory>

#include "../include/ISound.hpp"

class SoundBuffer;

class Sound
    : public ISound
    , public std::enable_shared_from_this<Sound>
{
private:
    friend class SoundBuffer;
    friend class SoundEngine;

protected:
    Sound(SoundBuffer* buffer, bool isAutoDelete);
    
    virtual bool IsValid() const = 0;

public:
//    virtual bool Play() = 0;
//    virtual bool Pause() = 0;
//    virtual bool Stop() = 0;
//    
//    virtual bool SetLoop(bool isLoop) = 0;
//    virtual bool IsLoopped() const = 0;
//    
//    virtual bool SetTime(float timeSeconds) = 0;
//    virtual float GetTime() const = 0;
//    
//    virtual bool IsPlaying() const = 0;
//    virtual bool IsPaused() const = 0;
//    virtual bool IsStopped() const = 0;
//    
//    virtual bool SetVolume(float volume) = 0;
//    virtual float GetVolume() const = 0;
//    
//    virtual float GetDurationSec() const = 0;
    
public:
    const std::string& GetFileName() const;
    
protected:
    ~Sound();
    void Delete() override final;

protected:    
    float m_currentTime;
    float m_saveCurrentTime;
    
    bool m_isLoop;
    float m_volume;
    
    float m_duration;
    bool m_isAutoDelete;
    
    SoundBuffer* m_buffer {nullptr};
};
