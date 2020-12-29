#pragma once

#ifdef X2AUDIO_SOUND

#include "../SoundBuffer.hpp"

#include <xaudio2.h>

struct InternalEngine;

class XAudio2Buffer final : public SoundBuffer<XAudio2Sound>
{
public:
    XAudio2Buffer(const std::string& fileName, InternalEngine* engine);

    const WAVEFORMATEX& GetWaveFormatEx() const { return m_wfEx; }
    XAUDIO2_BUFFER* GetX2Buffer() { return &m_xa2Buffer; }

    void AttachSource(XAudio2Sound* sound) override;
    void DetachSource(XAudio2Sound* sound) override;

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

