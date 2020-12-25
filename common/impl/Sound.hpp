#pragma once

#include <string>
#include <memory>

#include "../include/ISound.hpp"
#include "SoundBuffer.hpp"

//class SoundBuffer;
//class InternalEngine;
struct InternalBuffer;

class InternalSound : public SoundPtr
{

};

template<typename TSound>
class Sound : public InternalSound
{
    TSound* as_derived() noexcept
    {
        static_assert(std::is_base_of<Sound<TSound>, TSound>::value, "");
        return static_cast<TSound*>(this);
    }

protected:
    Sound(SoundBuffer<TSound>* buffer, bool isAutoDelete);

    virtual bool IsValid() const = 0;

public:
    const std::string& GetFileName() const;
    void Delete() final override;

protected:
    ~Sound();
  
protected:
    float m_currentTime;
    float m_saveCurrentTime;
    
    bool m_isLoop;
    float m_volume;
    
    float m_duration;
    bool m_isAutoDelete;

    SoundBuffer<TSound>* m_buffer{ nullptr };
};


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
Sound<TSound>::~Sound()
{
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
const std::string& Sound<TSound>::GetFileName() const
{
    return m_buffer->GetFileName();
}
