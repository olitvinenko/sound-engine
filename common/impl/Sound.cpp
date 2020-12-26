#ifndef __SOUND_CPP__
#define __SOUND_CPP__

#include "Sound.hpp"


template<typename TSound>
TSound* Sound<TSound>::as_derived() noexcept
{
    static_assert(std::is_base_of<Sound<TSound>, TSound>::value, "");
    return static_cast<TSound*>(this);
}

template<typename TSound>
Sound<TSound>::Sound(SoundBuffer<TSound>* buffer, bool isAutoDelete)
    : m_currentTime(0)
    , m_saveCurrentTime(0)
    , m_isLoop(false)
    , m_volume(100)
    , m_duration(buffer->Duration())
    , m_isAutoDelete(isAutoDelete)
    , m_buffer(buffer)
{
}

template<typename TSound>
const std::string& Sound<TSound>::GetFileName() const
{
    return m_buffer->GetFileName();
}

template<typename TSound>
void Sound<TSound>::Delete()
{
    if (!IsValid())
        return;
    
    if (IsPlaying())
        Stop();
    
    m_buffer->DetachSource(as_derived());
}

template<typename TSound>
Sound<TSound>::~Sound()
{
}

#endif // __SOUND_CPP__
