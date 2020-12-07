#ifdef OPENAL_SOUND

#include "OalSound.hpp"
#include "OalBuffer.hpp"

static ALuint GenerateSourceId()
{
    ALenum error = AL_NO_ERROR;
    error = alGetError();
    
    ALuint sourceID;
    // grab a source ID from openAL, this will be the base source ID
    alGenSources(1, &sourceID);
    error = alGetError();

    if ((error = alGetError()) != AL_NO_ERROR || sourceID == 0)
    {
        //("error: %i create sourceID: %i\n", error, sourceID);
        return 0;
    }
    
    return sourceID;
}

static void DeleteSourceId(ALuint sourceID)
{
    ALenum error = alGetError();

    alSourceStop(sourceID);
    alDeleteSources(1, &sourceID);
    
    if((error = alGetError()) != AL_NO_ERROR || sourceID==0)
    {
        //("error: %i Delete sourceID: %i\n", error, sourceID);
    }
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

    if (m_sourceID)
    {
        alGetError();
        alSourcePlay(m_sourceID);
        if (alGetError() != AL_NO_ERROR)
        {
            //todo:: log
            return false;
        }
        
        return true;
    }
    
    return false;
}

bool OalSound::Stop()
{
    if (!IsValid())
        return false;
    
    assert(alIsSource(m_sourceID));
    
    m_saveCurrentTime = 0;
    m_currentTime = 0;
    
    alGetError();
    alSourceStop(m_sourceID);
    if(alGetError() != AL_NO_ERROR)
    {
        //("error pause file:%s\n", m_file.data());
        return false;
    }
    
    return true;
}

bool OalSound::Pause()
{
    if (!IsValid())
        return false;
    
    m_saveCurrentTime = GetTime();
    
    //alGetSourcei(m_sourceID, AL_SOURCE_STATE, &m_saveState);
    //alGetSourcef(m_sourceID, AL_SEC_OFFSET, &m_saveCurrentTime);
    
    alGetError();
    alSourcePause(m_sourceID);
    if(alGetError() != AL_NO_ERROR)
    {
        //("error pause file:%s\n", m_file.data());
        return false;
    }
    
    return true;
}

bool OalSound::SetLoop(bool isLoop)
{
    m_isLoop = isLoop;
    
    if (m_sourceID)
    {
        alSourcei(m_sourceID, AL_LOOPING, m_isLoop ? AL_TRUE : AL_FALSE);
        return true;
    }
    
    return false;
}

bool OalSound::IsLoopped() const
{
    if (!m_sourceID)
        return false;
    
   ALint state(-1);
   alGetSourcei(m_sourceID, AL_LOOPING, &state);
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
    
    alGetError();
    alSourcef(m_sourceID, AL_SEC_OFFSET, timeSeconds);
    if(alGetError() != AL_NO_ERROR)
    {
        //("error SetCurrentTime file:%s\n", m_file.data());
        return false;
    }
    
    return true;
}

float OalSound::GetTime() const
{
    if (!m_sourceID)
        return 0.0f;
    
    ALfloat time;
    alGetSourcef(m_sourceID, AL_SEC_OFFSET, &time);
    return time;
}

bool OalSound::IsPlaying() const
{
    if (!m_sourceID)
        return false;
    
    ALenum state(-1);
    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool OalSound::IsPaused() const
{
    if (!m_sourceID)
        return false;
    
    ALenum state(-1);
    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}

bool OalSound::IsStopped() const
{
    if (!m_sourceID)
        return false;
    
    ALenum state(-1);
    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

bool OalSound::SetVolume(float volume)
{
    m_volume = std::max(std::min(volume, 1.0f), 0.0f);
    return Volume(m_volume);
}

bool OalSound::Volume(float volume)
{
    if (m_sourceID)
    {
        ALfloat vol = std::max(std::min(volume, 1.0f), 0.0f);
        
        alGetError();
        alSourcef(m_sourceID, AL_GAIN, vol);
        if(alGetError() != AL_NO_ERROR)
        {
            //("error SetVolume file:%s\n", m_file.data());
            return false;
        }
        return true;
    }
    return false;
}


#endif