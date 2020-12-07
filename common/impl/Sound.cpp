#include "Sound.hpp"

#include "SoundBuffer.hpp"
#include <cassert>

Sound::Sound(SoundBuffer* buffer, bool isAutoDelete)
        : m_currentTime(0)
        , m_saveCurrentTime(0)
        , m_isLoop(false)
        , m_volume(100)
        , m_duration(0)
        , m_isAutoDelete(isAutoDelete)
        , m_buffer(buffer)
{
    m_duration = m_buffer->Duration();
    m_buffer->AttachSource(this);
}

Sound::~Sound()
{
}

void Sound::Delete()
{
    if (!IsValid())
        return;
    
    if (IsPlaying())
        Stop();
    
    auto locked = shared_from_this();
    m_buffer->DetachSource(this);
    
    //1. 'locked' variable
    //2. locked in SoundHandle
    assert(locked.use_count() <= 2);
}

const std::string& Sound::GetFileName() const
{
    return m_buffer->GetFileName();
}
