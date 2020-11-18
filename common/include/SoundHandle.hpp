#pragma once

#include <memory>

struct Sound;

class SoundHandle
{
    friend struct SoundEngine;
    
public:
    SoundHandle(SoundHandle&& handle) noexcept = default;
    SoundHandle(const SoundHandle& handle) = default;
    ~SoundHandle();
    
    Sound* operator ->();
    operator Sound*();
    
private:
    SoundHandle(std::weak_ptr<Sound> sound);

private:
    std::weak_ptr<Sound> m_sound;
};
