#include "SoundHandle.hpp"
#include "Sound.hpp"

using SoundPtr = std::shared_ptr<Sound>;

SoundHandle::SoundHandle(std::weak_ptr<Sound> sound)
   : m_sound(sound)
{
    SoundPtr ptr = *this;
    if (ptr)
        ptr->m_handlesCount++;
}

Sound* SoundHandle::operator ->()
{
    SoundPtr ptr = *this;
    assert(ptr);
    return ptr.get();
}

SoundHandle& SoundHandle::operator=(SoundHandle&& handle) noexcept
{
    SoundPtr sound = *this;
    
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
    SoundPtr ptr = *this;
    if (ptr)
        ptr->m_handlesCount++;
}

SoundHandle& SoundHandle::operator=(const SoundHandle& handle)
{
    if (this == &handle)
        return *this;
    
    SoundPtr thisPtr = *this;
    SoundPtr otherPtr = handle.operator SoundPtr ();
    
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
    SoundPtr sound = *this;
    if (!sound)
        return;
    
    sound->m_handlesCount--;
    
    if (!sound->m_handlesCount)
        sound->Delete();
}
