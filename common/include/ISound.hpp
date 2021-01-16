#pragma once

struct ISound
{
    virtual bool Play() = 0;
    virtual bool Pause() = 0;
    virtual bool Stop() = 0;
    
    virtual bool SetLoop(bool isLoop) = 0;
    virtual bool IsLoopped() const = 0;
    
    virtual bool SetTime(float timeSeconds) = 0;
    virtual float GetTime() const = 0;
    
    virtual bool IsPlaying() const = 0;
    virtual bool IsPaused() const = 0;
    virtual bool IsStopped() const = 0;
    
    virtual bool SetVolume(float volume) = 0;
    virtual float GetVolume() const = 0;
    
    virtual float GetDurationSec() const = 0;

    virtual void SetPitch(float pitch) = 0;

protected:
    virtual ~ISound() = default;
};

class SoundPtr : public ISound
{
    friend class SoundHandle;

protected:
    virtual void Delete() = 0;

private:
    std::size_t m_handlesCount{ 0 };
};