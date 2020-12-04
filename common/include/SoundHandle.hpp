#pragma once

#include <memory>

struct ISound;

class SoundHandle
{
    friend struct ISoundEngine;
    
public:
    SoundHandle(const SoundHandle& handle);
    SoundHandle& operator=(const SoundHandle& handle);
    
    SoundHandle(SoundHandle&& handle) noexcept = default;
    SoundHandle& operator=(SoundHandle&& handle) noexcept;
    
    ~SoundHandle();
    
    ISound* operator ->();
    operator bool() const { return !m_sound.expired(); };
    
private:
    operator std::shared_ptr<ISound>() const { return m_sound.lock(); };
    
    SoundHandle(std::weak_ptr<ISound> sound);

private:
    std::weak_ptr<ISound> m_sound;
};
