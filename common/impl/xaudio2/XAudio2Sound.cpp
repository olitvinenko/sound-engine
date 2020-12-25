#ifdef X2AUDIO_SOUND

#include "XAudio2Sound.hpp"
#include "XAudio2Buffer.hpp"

#define OPSETID 1U

XAudio2Sound::XAudio2Sound(IXAudio2* xa2, XAudio2Buffer* buffer, bool isAutoDelete)
    : Sound(buffer, isAutoDelete)
{
    HRESULT hr = xa2->CreateSourceVoice(&m_source, &buffer->GetWaveFormatEx());
    if (FAILED(hr))
        return;

    //m_buffer->AttachSource(this);
}

bool XAudio2Sound::Play()
{
    if (!IsValid())
        return false;

    if (IsPlaying())
        return false;

    SetLoop(m_isLoop);
    SetVolume(m_volume);

    return SUCCEEDED(m_source->Start(0, OPSETID));
}

bool XAudio2Sound::Pause() { return false; }
bool XAudio2Sound::Stop() { return false; }

bool XAudio2Sound::SetLoop(bool isLoop) { return false; }
bool XAudio2Sound::IsLoopped() const { return false; }

bool XAudio2Sound::SetTime(float timeSeconds) { return false; }
float XAudio2Sound::GetTime() const { return .0f; }

bool XAudio2Sound::IsPlaying() const { return false; }
bool XAudio2Sound::IsPaused() const { return false; }
bool XAudio2Sound::IsStopped() const { return false; }

bool XAudio2Sound::SetVolume(float volume) { return false; }
float XAudio2Sound::GetVolume() const { return .0f; }

float XAudio2Sound::GetDurationSec() const { return .0f; }

#endif
