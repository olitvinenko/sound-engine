#pragma once

#include <string>

#include "SoundBuffer.hpp"
#include "InternalSound.hpp"

template<typename TSound>
class Sound : public InternalSound
{
    TSound* as_derived() noexcept;

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

#include "Sound.cpp"
