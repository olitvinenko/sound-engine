#pragma once

#ifdef X2AUDIO_SOUND

#include "../SoundBuffer.hpp"

#include <xaudio2.h>

class SoundEngine;

class XAudio2Buffer final : public SoundBuffer
{
public:
    XAudio2Buffer(const std::string& fileName, SoundEngine* engine);

    void AttachSource(Sound* sound) override;
    void DetachSource(Sound* sound) override;

    const WAVEFORMATEX& GetWaveFormatEx() const { return m_wfEx; }

private:
    bool LoadBuffer();

private:
    bool LoadMemory() override { return LoadBuffer(); }
    bool IsActive() const override { return true; }

private:
    WAVEFORMATEX m_wfEx;
    std::vector<BYTE> m_data;
    XAUDIO2_BUFFER m_xa2Buffer;
};

#endif

