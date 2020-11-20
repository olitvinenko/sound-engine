#pragma once

struct Sound
{
    friend class SoundHandle;
    
    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
    
    virtual void SetLoop(bool isLoop) = 0;
    virtual bool IsLoopped() const = 0;
    
    virtual void SetTime(float timeSeconds) = 0;
    virtual float GetTime() const = 0;
    
    virtual bool IsPlaying() const = 0;
    virtual bool IsPaused() const = 0;
    virtual bool IsStopped() const = 0;
    
    virtual void SetVolume(float volume) = 0;
    virtual float GetVolume() const = 0;
    
    virtual float GetDurationSec() const = 0;
    
protected:
    virtual ~Sound() = default;
    virtual void Delete() = 0;
    
private:
    unsigned int m_handlesCount { 0 };
};
