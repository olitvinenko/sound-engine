#pragma once

#ifdef X2AUDIO_SOUND

#include "../Sound.hpp"

class XAudio2Buffer;

struct IXAudio2;
struct IXAudio2SourceVoice;

class XAudio2Sound final : public Sound
{
    friend class XAudio2Engine;
    
private:
    XAudio2Sound(IXAudio2* xa2, XAudio2Buffer* buffer, bool isAutoDelete);

public:
    IXAudio2SourceVoice* GetSourceVoice() const { return m_source; }
    
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
    
    float GetDurationSec() const override;
    
    bool IsValid() const override { return m_source; }

private:
    IXAudio2SourceVoice* m_source{ nullptr };
};

#endif
