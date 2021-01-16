#pragma once

#ifdef X2AUDIO_SOUND

#include <xaudio2.h>
#include <bitset>

#include "../Sound.hpp"

class XAudio2Buffer;

class XAudio2Sound final
	: public Sound<XAudio2Sound>
	, public IXAudio2VoiceCallback
{
    friend class XAudio2Engine;

    template<typename, typename>
    friend class SoundEngine;
    
private:
    XAudio2Sound(IXAudio2* xa2, XAudio2Buffer* buffer, bool isAutoDelete);

public:
    IXAudio2SourceVoice* GetSourceVoice() const { return m_source.get(); }

private: //ISound
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
    
    bool IsValid() const override { return static_cast<bool>(m_source) && m_x2Buffer; }

    void SetPitch(float pitch) override;

private: //IXAudio2VoiceCallback
    void OnVoiceProcessingPassStart(UINT32 BytesRequired) override;
    void OnVoiceProcessingPassEnd() override;
    void OnStreamEnd() override;
    void OnBufferStart(void* pBufferContext) override;
    void OnBufferEnd(void* pBufferContext) override;
    void OnLoopEnd(void* pBufferContext) override;
    void OnVoiceError(void* pBufferContext, HRESULT Error) override;

private:
    struct SourceDeleter
    {
        void operator()(IXAudio2SourceVoice*);
    };

    std::unique_ptr<IXAudio2SourceVoice, SourceDeleter> m_source;
    XAudio2Buffer* m_x2Buffer { nullptr };

    enum StateIndices
    {
	    PAUSED = 0,
        PLAYING,
        LOOPED,
        STOPPED,

        SIZE
    };

    std::bitset<StateIndices::SIZE> m_state;
    INT32 m_shiftedSamplesCount { 0 };
};

#endif
