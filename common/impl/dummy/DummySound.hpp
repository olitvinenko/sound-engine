#pragma once

#ifdef DUMMY_SOUND

#include "../Sound.hpp"

class DummyBuffer;

class DummySound final : public Sound<DummySound>
{
    friend class DummyEngine;
    
    template<typename, typename>
    friend class SoundEngine;
    
private:
    DummySound(DummyBuffer* buffer, bool isAutoDelete);
    
private:
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
    float GetVolume() const override;
    
    bool IsValid() const override;

    void SetPitch(float pitch) override;
};

#endif
