#ifdef X2AUDIO_SOUND

#include "XAudio2Sound.hpp"
#include "XAudio2Buffer.hpp"

XAudio2Sound::XAudio2Sound(XAudio2Buffer* buffer, bool isAutoDelete)
    : Sound(buffer, isAutoDelete)
{
}

bool XAudio2Sound::Play() { return false; }
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

bool XAudio2Sound::IsValid() const { return true; }

#endif
