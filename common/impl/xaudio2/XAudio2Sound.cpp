
#ifdef X2AUDIO_SOUND

#include "XAudio2Sound.hpp"
#include "XAudio2Buffer.hpp"

#include "XAudio2Utils.hpp"

void XAudio2Sound::SourceDeleter::operator()(IXAudio2SourceVoice* voice)
{
   voice->DestroyVoice();
}

XAudio2Sound::XAudio2Sound(IXAudio2* xa2, XAudio2Buffer* buffer, bool isAutoDelete)
    : Sound(buffer, isAutoDelete)
	, m_x2Buffer(buffer)
{
    IXAudio2SourceVoice* voice;
	const auto callResult = x2WrapCall(xa2->CreateSourceVoice(&voice, &buffer->GetWaveFormatEx(), 0, 2, this));
    if (!callResult)
        return;

    m_source.reset(voice);
}

bool XAudio2Sound::Play()
{
    if (!IsValid())
        return false;

    if (IsPlaying())
        return false;

    if (!m_state.test(PAUSED))
    //if (!m_isPaused)
    {
        SetLoop(m_isLoop);
        SetVolume(m_volume);

        m_source->FlushSourceBuffers();
        m_source->SubmitSourceBuffer(m_x2Buffer->GetX2Buffer());
    }

    m_state.reset(PAUSED);
    m_state.set(PLAYING);

    //m_isPaused = false;
    //m_isPlaying = true;

    return x2WrapCall(m_source->Start());
}

bool XAudio2Sound::Pause()
{
    if(!IsValid())
        return false;

    m_state.reset(PLAYING);
    m_state.set(PAUSED);
    
    //m_isPaused = true;
    //m_isPlaying = false;

    return x2WrapCall(m_source->Stop());
}

bool XAudio2Sound::Stop()
{
    if (!IsValid())
        return false;

    if (IsStopped())
        return false;

    m_state.reset(PLAYING);
    m_state.set(STOPPED);
    //m_isPlaying = false;

	return x2WrapCall(m_source->Stop());
}

bool XAudio2Sound::SetLoop(bool isLoop)
{
    isLoop ? m_state.set(LOOPED) : m_state.reset(LOOPED);

    //m_isLoop = isLoop;
	return true;
}

bool XAudio2Sound::IsLoopped() const
{
    return m_state.test(LOOPED);
	//return m_isLoop;
}

bool XAudio2Sound::IsPlaying() const
{
    if (!IsValid())
        return false;

    XAUDIO2_VOICE_STATE state;
    m_source->GetState(&state);

    return state.SamplesPlayed > 0 && state.BuffersQueued > 0 && m_state.test(PLAYING);// m_isPlaying;
}

bool XAudio2Sound::IsPaused() const { return m_state.test(PAUSED); }
bool XAudio2Sound::IsStopped() const { return m_state.test(STOPPED) && !IsPlaying(); }

//TODO::
bool XAudio2Sound::SetTime(float timeSeconds) { return false; }

//TODO::
float XAudio2Sound::GetTime() const { return .0f; }

//TODO::
bool XAudio2Sound::SetVolume(float volume) { return false; }
//TODO::
float XAudio2Sound::GetVolume() const { return .0f; }

//TODO::
float XAudio2Sound::GetDurationSec() const { return .0f; }

//----------------------------------------------------------------------

void XAudio2Sound::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{ }

void XAudio2Sound::OnVoiceProcessingPassEnd()
{ }

void XAudio2Sound::OnLoopEnd(void* pBufferContext)
{
}


void XAudio2Sound::OnStreamEnd()
{
    m_state.reset(PLAYING);

   // m_isPlaying = false;

    if (m_isLoop)
        Play();
}

void XAudio2Sound::OnBufferStart(void* pBufferContext)
{
    m_state.set(PLAYING);
    //m_isPlaying = true;
}

void XAudio2Sound::OnBufferEnd(void* pBufferContext)
{
    m_state.reset(PLAYING);
   // m_isPlaying = false;
}

// Called in the event of a critical error during voice processing,
// such as a failing xAPO or an error from the hardware XMA decoder.
// The voice may have to be destroyed and re-created to recover from
// the error.  The callback arguments report which buffer was being
// processed when the error occurred, and its HRESULT code.
void XAudio2Sound::OnVoiceError(void* pBufferContext, HRESULT Error)
{
    //TODO:: recreate and restore state
}

#endif
