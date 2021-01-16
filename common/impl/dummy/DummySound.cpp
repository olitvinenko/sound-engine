#ifdef DUMMY_SOUND

#include "DummySound.hpp"
#include "DummyBuffer.hpp"

DummySound::DummySound(DummyBuffer* buffer, bool isAutoDelete)
    : Sound(buffer, isAutoDelete)
{
    
}

bool DummySound::Play() { return false; }
bool DummySound::Pause() { return false; }
bool DummySound::Stop() { return false; }

bool DummySound::SetLoop(bool isLoop) { return false; }
bool DummySound::IsLoopped() const { return false; }

bool DummySound::SetTime(float timeSeconds) { return false; }
float DummySound::GetTime() const { return .0f; }

bool DummySound::IsPlaying() const { return false; }
bool DummySound::IsPaused() const { return false; }
bool DummySound::IsStopped() const { return false; }

bool DummySound::SetVolume(float volume) { return false; }
float DummySound::GetVolume() const { return .0f; }

bool DummySound::IsValid() const { return true; }

void DummySound::SetPitch(float pitch) { }

#endif
