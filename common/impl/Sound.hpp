#pragma once

#include <string>
#include <memory>

#include "../include/ISound.hpp"

class SoundBuffer;

class Sound
    : public ISound
    , public std::enable_shared_from_this<Sound>
{
private:
    friend class SoundBuffer;
    friend class SoundEngine;

protected:
    Sound(SoundBuffer* buffer, bool isAutoDelete);
    
    virtual bool IsValid() const = 0;

public:
    const std::string& GetFileName() const;
    
protected:
    ~Sound();
    void Delete() override final;

protected:    
    float m_currentTime;
    float m_saveCurrentTime;
    
    bool m_isLoop;
    float m_volume;
    
    float m_duration;
    bool m_isAutoDelete;
    
    SoundBuffer* m_buffer {nullptr};
};
