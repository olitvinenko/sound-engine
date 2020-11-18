#include "SoundHandle.hpp"
#include "Sound.hpp"

SoundHandle::SoundHandle(std::weak_ptr<Sound> sound)
   : m_sound(sound)
{
}

Sound* SoundHandle::operator ->()
{
    Sound* ptr = *this;
    assert(ptr);
    return ptr;
}

SoundHandle::operator Sound*()
{
    if (auto ptr = m_sound.lock())
        return ptr.get();

    return nullptr;
}

SoundHandle::~SoundHandle()
{
    Sound* ptr = *this;
    if (ptr && ptr->IsPlaying())
        ptr->Stop();
}
