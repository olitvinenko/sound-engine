#ifdef SOUND_OPENAL

#include "OalSound.hpp"
#include "OalBuffer.hpp"
#include "OalUtils.hpp"

static ALuint GenerateSourceId()
{
    ALuint sourceID;
    
    if (alCall(alGenSources, 1, &sourceID))
        return sourceID;
    
    return 0;
}

static void DeleteSourceId(ALuint sourceID)
{
    alCall(alSourceStop, sourceID);
    alCall(alDeleteSources, 1, &sourceID);
}

OalSound::OalSound(OalBuffer* buffer, bool isAutoDelete)
        : Sound(buffer, isAutoDelete)
        , m_sourceID(0)
{
    m_sourceID = GenerateSourceId();
}
    
OalSound::~OalSound()
{
    if (m_sourceID)
        DeleteSourceId(m_sourceID);
    
    m_sourceID = 0;
}

bool OalSound::Play()
{
    if (IsPlaying())
        return false;
    
    SetLoop(m_isLoop);
    SetVolume(m_volume);

    //TODO:: check saved time for stopped/paused states

    if (m_sourceID && alCall(alSourcePlay, m_sourceID))
    {
        return true;
    }
    
    return false;
}

bool OalSound::Stop()
{
    if (!IsValid())
        return false;
    
    assert(alCall(alIsSource, m_sourceID).value);
    
    m_saveCurrentTime = 0;
    m_currentTime = 0;
    
    return alCall(alSourceStop, m_sourceID);
}

bool OalSound::Pause()
{
    if (!IsValid())
        return false;
    
    m_saveCurrentTime = GetTime();
    
    //alGetSourcei(m_sourceID, AL_SOURCE_STATE, &m_saveState);
    //alGetSourcef(m_sourceID, AL_SEC_OFFSET, &m_saveCurrentTime);
    
    return alCall(alSourcePause, m_sourceID);
}

bool OalSound::SetLoop(bool isLoop)
{
    m_isLoop = isLoop;
    
    if (IsValid())
    {
        return alCall(alSourcei, m_sourceID, AL_LOOPING, m_isLoop ? AL_TRUE : AL_FALSE);
    }
    
    return false;
}

bool OalSound::IsLoopped() const
{
    if (!m_sourceID)
        return false;
    
    ALint state(-1);
    alCall(alGetSourcei, m_sourceID, AL_LOOPING, &state);
    return state == AL_TRUE && m_isLoop;
}

bool OalSound::SetTime(float timeSeconds)
{
    if (!m_sourceID)
        return false;
    
    if (timeSeconds < 0)
        timeSeconds = 0;
    else if (timeSeconds > m_duration)
        timeSeconds = m_duration;
    
    return alCall(alSourcef, m_sourceID, AL_SEC_OFFSET, timeSeconds);
}

float OalSound::GetTime() const
{
    if (!m_sourceID)
        return 0.0f;
    
    ALfloat time;
    alCall(alGetSourcef, m_sourceID, AL_SEC_OFFSET, &time);
    return time;
}

bool OalSound::IsPlaying() const
{
    if (!m_sourceID)
        return false;
    
    ALenum state(-1);
    alCall(alGetSourcei, m_sourceID, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool OalSound::IsPaused() const
{
    if (!m_sourceID)
        return false;
    
    ALenum state(-1);
    alCall(alGetSourcei, m_sourceID, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}

bool OalSound::IsStopped() const
{
    if (!m_sourceID)
        return false;
    
    ALenum state(-1);
    alCall(alGetSourcei, m_sourceID, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

bool OalSound::SetVolume(float volume)
{
    m_volume = std::max(std::min(volume, 1.0f), 0.0f);
    return Volume(m_volume);
}

bool OalSound::Volume(float volume)
{
    if (IsValid())
    {
        ALfloat vol = std::max(std::min(volume, 1.0f), 0.0f);
        return alCall(alSourcef, m_sourceID, AL_GAIN, vol);
    }
    return false;
}

void OalSound::SetPitch(float pitch)
{
    if (!IsValid())
        return;

    alCall(alSourcef, m_sourceID, AL_PITCH, pitch);
}

#endif
