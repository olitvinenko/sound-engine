#include "OalSound.hpp"
#include "OalBuffer.hpp"

OalSound::OalSound(std::shared_ptr<OalBuffer> buffer, bool isAutoDelete)
        : m_currentTime(0)
        , m_saveCurrentTime(0)
        , m_isLoop(false)
        , m_volume(1)
        , m_duration(buffer->Duration())
        , m_isAutoDelete(isAutoDelete)
        , m_sourceID(0)
        , m_buffer(buffer)
{
}
    
OalSound::~OalSound()
{
    Stop();
}

const std::string& OalSound::GetFileName() const
{
    static const std::string empty;
    if (!m_buffer)
        return empty;
    
    return m_buffer->GetFileName();
}

void OalSound::UnloadBuffer()
{
    if (m_sourceID)
    {
        //alGetSourcei(m_sourceID, AL_SOURCE_STATE, &m_saveState);
        //alGetSourcef(m_sourceID, AL_SEC_OFFSET, &m_saveCurrentTime);
    }
    m_sourceID = 0;
    m_currentTime = 0;
   // mIsUnloadBuffer = true;
}

void OalSound::Play()
{
    if (IsPlaying())
        return;

    if (m_buffer && !m_sourceID)
    {
        m_sourceID = m_buffer->GetSource(this);
        SetLoop(m_isLoop);
        
        m_volume = 100;
        SetVolume(m_volume);
    }

    if (m_sourceID)
    {
        alGetError();
        alSourcePlay(m_sourceID);
        if (alGetError() != AL_NO_ERROR)
        {
            //todo:: log
        }
    }

}

void OalSound::Pause()
{
    if (m_sourceID)
    {
        m_saveCurrentTime = GetTime();
        
        alGetError();
        alSourcePause(m_sourceID);
        if(alGetError() != AL_NO_ERROR)
        {
            //("error pause file:%s\n", m_file.data());
            return;
        }
    }
}

void OalSound::Stop()
{
    if (!m_sourceID)
        return;
    
    m_saveCurrentTime = 0;
    
    alGetError();
    alSourceStop(m_sourceID);
    if(alGetError() != AL_NO_ERROR)
    {
        //("error stop file:%s\n", m_file.data());
        return;
    }
    
    if (m_buffer)
    {
        m_buffer->RemoveSource(this);
        m_sourceID = 0;
    }
}

void OalSound::SetLoop(bool isLoop)
{
    m_isLoop = isLoop;
    
    if (m_sourceID)
    {
        alSourcei(m_sourceID, AL_LOOPING, m_isLoop ? AL_TRUE : AL_FALSE);
    }
}

bool OalSound::IsLoopped() const
{
    if (m_sourceID)
    {
        ALint state(-1);
        alGetSourcei(m_sourceID, AL_LOOPING, &state);
        return state == AL_TRUE && m_isLoop;
    }
    
    return false;
}

void OalSound::SetTime(float timeSeconds)
{
    if (!m_sourceID)
        return;
    
    if (timeSeconds < 0)
        timeSeconds = 0;
    else if (timeSeconds > m_duration)
        timeSeconds = m_duration;
    
    alGetError();
    alSourcef(m_sourceID, AL_SEC_OFFSET, timeSeconds);
    if(alGetError() != AL_NO_ERROR)
    {
        //("error SetCurrentTime file:%s\n", m_file.data());
        return;
    }
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
    if (m_sourceID)
    {
        ALenum state(-1);
        alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }
    return false;
}

bool OalSound::IsPaused() const
{
    if (m_sourceID)
    {
        ALenum state(-1);
        alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
        return state == AL_PAUSED;
    }
    return false;
}

bool OalSound::IsStopped() const
{
    if (m_sourceID)
    {
        ALenum state(-1);
        alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
        return state == AL_STOPPED;
    }
    return false;
}

void OalSound::SetVolume(float volume)
{
    m_volume = std::max(std::min(volume, 1.0f), 0.0f);
    Volume(m_volume);
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
            //ALOG("error SetVolume file:%s\n", m_file.data());
            return false;
        }
        return true;
    }
    return false;
}
