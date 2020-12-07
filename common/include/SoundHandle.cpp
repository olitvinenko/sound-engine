#include "SoundHandle.hpp"
#include "ISound.hpp"
#include <cassert>

using sound_ptr = std::shared_ptr<ISound>;

SoundHandle::SoundHandle(std::weak_ptr<ISound> sound)
   : m_sound(sound)
{
    sound_ptr ptr = *this;
    if (ptr)
        ptr->m_handlesCount++;
}

ISound* SoundHandle::operator ->()
{
    sound_ptr ptr = *this;
    assert(ptr);
    return ptr.get();
}

SoundHandle& SoundHandle::operator=(SoundHandle&& handle) noexcept
{
    sound_ptr sound = *this;
    
    if (sound)
    {
        sound->m_handlesCount--;
        if (!sound->m_handlesCount)
            sound->Delete();
    }
    
    m_sound = std::move(handle.m_sound);
    return *this;
}

SoundHandle::SoundHandle(const SoundHandle& handle)
    : m_sound(handle.m_sound)
{
    sound_ptr ptr = *this;
    if (ptr)
        ptr->m_handlesCount++;
}

SoundHandle& SoundHandle::operator=(const SoundHandle& handle)
{
    if (this == &handle)
        return *this;
    
    sound_ptr thisPtr = *this;
    sound_ptr otherPtr = handle.operator sound_ptr ();
    
    if (thisPtr == otherPtr)
        return *this;
    
    if (thisPtr)
    {
        thisPtr->m_handlesCount--;
        if (!thisPtr->m_handlesCount)
            thisPtr->Delete();
    }
    
    m_sound = handle.m_sound;
    
    thisPtr = *this;
    if (thisPtr)
        thisPtr->m_handlesCount++;
    
    return *this;
}

SoundHandle::~SoundHandle()
{
    sound_ptr sound = *this;
    if (!sound)
        return;
    
    sound->m_handlesCount--;
    
    if (!sound->m_handlesCount)
        sound->Delete();
}
