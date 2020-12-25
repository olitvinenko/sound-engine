#pragma once

#include <memory>

struct ISound;
class SoundPtr;

class SoundHandle
{
public:
    SoundHandle(std::weak_ptr<SoundPtr> sound);

    SoundHandle(const SoundHandle& handle);
    SoundHandle& operator=(const SoundHandle& handle);
    
    SoundHandle(SoundHandle&& handle) noexcept = default;
    SoundHandle& operator=(SoundHandle&& handle) noexcept;
    
    ~SoundHandle();
    
    ISound* operator ->();
    operator bool() const { return !m_sound.expired(); };
    
private:
    operator std::shared_ptr<SoundPtr>() const { return m_sound.lock(); };
    
private:
    std::weak_ptr<SoundPtr> m_sound;
};


