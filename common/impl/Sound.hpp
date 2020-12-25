#pragma once

#include <string>
#include <memory>

#include "SoundBuffer.hpp"
#include "InternalSound.hpp"

template<typename TSound>
class Sound : public InternalSound
{
    TSound* as_derived() noexcept
    {
        static_assert(std::is_base_of<Sound<TSound>, TSound>::value, "");
        return static_cast<TSound*>(this);
    }

protected:
    Sound(SoundBuffer<TSound>* buffer, bool isAutoDelete)
        : m_currentTime(0)
        , m_saveCurrentTime(0)
        , m_isLoop(false)
        , m_volume(100)
        , m_duration(buffer->Duration())
        , m_isAutoDelete(isAutoDelete)
        , m_buffer(buffer)
    {
    }

    virtual bool IsValid() const = 0;

public:
    const std::string& GetFileName() const
    {
        return m_buffer->GetFileName();
    }
    
    void Delete() final override
    {
        if (!IsValid())
            return;
        
        if (IsPlaying())
            Stop();
        
        m_buffer->DetachSource(as_derived());
    }

protected:
    ~Sound()
    {
    }
  
protected:
    float m_currentTime;
    float m_saveCurrentTime;
    
    bool m_isLoop;
    float m_volume;
    
    float m_duration;
    bool m_isAutoDelete;

    SoundBuffer<TSound>* m_buffer{ nullptr };
};

