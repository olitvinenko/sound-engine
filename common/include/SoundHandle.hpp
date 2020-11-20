#pragma once

#include <memory>

struct Sound;

class SoundHandle
{
    friend struct SoundEngine;
    
public:
    SoundHandle(SoundHandle&& handle) noexcept;
    SoundHandle& operator=(SoundHandle&& handle) noexcept;
    
    SoundHandle(const SoundHandle& handle);
    SoundHandle& operator=(const SoundHandle& handle);
    
    ~SoundHandle();
    
    Sound* operator ->();
    operator bool() const { return !m_sound.expired(); };
    
private:
    operator std::shared_ptr<Sound>() const { return m_sound.lock(); };
    
    SoundHandle(std::weak_ptr<Sound> sound);

private:
    std::weak_ptr<Sound> m_sound;
};
