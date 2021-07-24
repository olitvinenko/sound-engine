#ifdef SOUND_X2AUDIO

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
    {
        SetLoop(m_state.test(LOOPED));
        SetVolume(m_volume);

        m_source->FlushSourceBuffers();
        m_source->SubmitSourceBuffer(m_x2Buffer->GetX2Buffer());
    }

    m_state.reset(PAUSED);
    m_state.reset(STOPPED);
    m_state.set(PLAYING);

    return x2WrapCall(m_source->Start());
}

bool XAudio2Sound::Pause()
{
    if(!IsValid())
        return false;

    m_state.reset(PLAYING);
    m_state.set(PAUSED);

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

    m_shiftedSamplesCount = 0;

    return x2WrapCall(m_source->Stop());
}

bool XAudio2Sound::SetLoop(bool isLoop)
{
    isLoop ? m_state.set(LOOPED) : m_state.reset(LOOPED);

	return true;
}

bool XAudio2Sound::IsLoopped() const
{
    return m_state.test(LOOPED);
}

bool XAudio2Sound::IsPlaying() const
{
    if (!IsValid())
        return false;

    XAUDIO2_VOICE_STATE state;
    m_source->GetState(&state);

    return state.pCurrentBufferContext && state.SamplesPlayed > 0 && state.BuffersQueued > 0 && m_state.test(PLAYING);// m_isPlaying;
}

bool XAudio2Sound::IsPaused() const { return m_state.test(PAUSED); }
bool XAudio2Sound::IsStopped() const { return m_state.test(STOPPED) && !IsPlaying(); }

bool XAudio2Sound::SetTime(float timeSeconds)
{
    if (IsPlaying())
        Stop();

    XAUDIO2_VOICE_STATE state;
    m_source->GetState(&state);

    const UINT32 samplesShifted = static_cast<UINT32>(timeSeconds * static_cast<float>(m_x2Buffer->GetWaveFormatEx().nSamplesPerSec));

    XAUDIO2_BUFFER* buffer = m_x2Buffer->GetX2Buffer();

    buffer->PlayBegin = samplesShifted;
    Play();
    buffer->PlayBegin = 0;

    m_shiftedSamplesCount = state.SamplesPlayed - samplesShifted;

	return true;
}

float XAudio2Sound::GetTime() const
{
    if (!IsValid())
        return .0f;

    XAUDIO2_VOICE_STATE state;
    m_source->GetState(&state);

    return static_cast<float>(state.SamplesPlayed - m_shiftedSamplesCount) / static_cast<float>(m_x2Buffer->GetWaveFormatEx().nSamplesPerSec);
}

bool XAudio2Sound::SetVolume(float volume)
{
    if (!IsValid())
        return false;

    m_volume = max(min(volume, 1.0f), 0.0f);
    return x2WrapCall(m_source->SetVolume(m_volume));
}

float XAudio2Sound::GetVolume() const
{
    if (!IsValid())
        return .0f;

    m_source->GetVolume(const_cast<float*>(&m_volume));
    return m_volume;
}

void XAudio2Sound::SetPitch(float pitch)
{
    if (!IsValid())
        return;

    x2WrapCall(m_source->SetFrequencyRatio(pitch));
}

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

    m_shiftedSamplesCount = 0;

    if (m_state.test(LOOPED))
        Play();
}

void XAudio2Sound::OnBufferStart(void* pBufferContext)
{
    m_state.set(PLAYING);
}

void XAudio2Sound::OnBufferEnd(void* pBufferContext)
{
    XAUDIO2_VOICE_STATE state;
    m_source->GetState(&state);

    if (!state.BuffersQueued)
        m_state.reset(PLAYING);
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
