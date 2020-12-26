#ifndef __SOUND_BUFFER_CPP__
#define __SOUND_BUFFER_CPP__

#include "SoundBuffer.hpp"


template<typename TSound>
SoundBuffer<TSound>::SoundBuffer(const std::string& fileName, InternalEngine* engine)
    : m_fileName(fileName)
    , m_soundEngine(engine)
{
}

template<typename TSound>
SoundBuffer<TSound>::~SoundBuffer()
{
    UnloadAllSources();
}


template<typename TSound>
bool SoundBuffer<TSound>::CanBeErased() const
{
    if (!m_sources.empty())
        return false;

    if (!IsActive())
        return false;

    return true;
}

template<typename TSound>
void SoundBuffer<TSound>::AttachSource(TSound* sound)
{
    m_sources.insert(sound);
    m_soundEngine->OnSourceCreated(this, sound);
}

template<typename TSound>
void SoundBuffer<TSound>::DetachSource(TSound* sound)
{
    m_sources.erase(sound);
    m_soundEngine->OnSourceRemoved(this, sound);
}

template<typename TSound>
void SoundBuffer<TSound>::UnloadAllSources()
{
    auto it = m_sources.begin();
    while (it != m_sources.end())
    {
        (*it)->Delete();
        it = m_sources.begin();
    }

    assert(m_sources.empty());
}

#endif //__SOUND_BUFFER_CPP__
