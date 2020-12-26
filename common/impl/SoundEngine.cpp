#ifndef __SOUND_ENGINE_CPP__
#define __SOUND_ENGINE_CPP__

#include "SoundEngine.hpp"

template<typename TSound, typename TBuffer>
SoundEngine<TSound,TBuffer>::SoundEngine()
    : m_maxMem(15.f)
    , m_curMem(0.f)
{
    static_assert(std::is_base_of<InternalSound, TSound>::value, "");
    static_assert(std::is_base_of<InternalBuffer, TBuffer>::value, "");
}

template<typename TSound, typename TBuffer>
SoundEngine<TSound,TBuffer>::~SoundEngine()
{
    Reset();
}

template<typename TSound, typename TBuffer>
SoundHandle SoundEngine<TSound,TBuffer>::PlaySound(const std::string& filePath, bool isAutoDelete)
{
    SoundPtr sound = CreateSound(filePath, isAutoDelete);
    sound->Play();
    return SoundHandle(sound);
}

template<typename TSound, typename TBuffer>
SoundHandle SoundEngine<TSound,TBuffer>::GetSound(const std::string& filePath, bool isAutoDelete)
{
    std::shared_ptr<TSound> sound = CreateSound(filePath, isAutoDelete);
    return SoundHandle(sound);
}

template<typename TSound, typename TBuffer>
bool SoundEngine<TSound,TBuffer>::PlayOnce(const std::string& filePath)
{
    SoundPtr sound = CreateSound(filePath, true);
    sound->Play();
    return sound->IsPlaying();
}

template<typename TSound, typename TBuffer>
void SoundEngine<TSound,TBuffer>::Update(float deltaTime)
{
    auto it = m_sounds.begin();
    while (it != m_sounds.end())
    {
        SoundsList& list = it->second;

        for (std::size_t i = 0; i < list.size();)
        {
            if (list[i]->m_isAutoDelete && !list[i]->IsPlaying() && list[i]->IsValid()) // played once
                list[i]->Delete();
            else
                ++i;
        }

        ++it;
    }
}

template<typename TSound, typename TBuffer>
bool SoundEngine<TSound,TBuffer>::ActivateBuffer(BufferPtr buffer)
{
    if (buffer->IsActive())
        return true;

    if (!buffer->LoadMemory())
        return false;

    if (buffer->SizeMem() >= GetMaxMem()) // too big file
    {
        buffer->UnloadAllSources();

#ifndef NDEBUG
        assert(false);
#endif
        return false;
    }

    IncrementMemory(buffer->SizeMem());

    if (GetCurMem() >= GetMaxMem())
    {
        for (auto it = m_buffers.begin(); it != m_buffers.end(); ++it)
        {
            auto buffer = it->second;
            auto& sources = buffer->m_sources;

            bool anyPlaying = std::any_of(sources.begin(), sources.end(), [](TSound* it)
                {
                    return it->IsPlaying();
                });

            if (anyPlaying || !buffer->CanBeErased())
                continue;

            buffer->UnloadAllSources();
        }
    }


    if (GetCurMem() >= GetMaxMem())
    {
#ifndef NDEBUG
        assert(false);
#endif
        return false;
    }

    return true;
}

template<typename TSound, typename TBuffer>
bool SoundEngine<TSound,TBuffer>::DeactivateBuffer(BufferPtr buffer)
{
    DecrementMemory(buffer->m_sizeMemory);
    return true;
}

template<typename TSound, typename TBuffer>
void SoundEngine<TSound,TBuffer>::IncrementMemory(float sizeMem)
{
    m_curMem += sizeMem;
}

template<typename TSound, typename TBuffer>
void SoundEngine<TSound,TBuffer>::DecrementMemory(float sizeMem)
{
    m_curMem -= sizeMem;
    assert(m_curMem >= 0);
}

template<typename TSound, typename TBuffer>
void SoundEngine<TSound,TBuffer>::OnSourceCreated(InternalBuffer* buffer, InternalSound* sound)
{
    IncrementMemory(buffer->GetSizeMem());
    //TODO::
}

template<typename TSound, typename TBuffer>
void SoundEngine<TSound,TBuffer>::OnSourceRemoved(InternalBuffer* buffer, InternalSound* sound)
{
    assert(!m_sounds.empty());

    auto bufferIt = m_sounds.find(buffer->GetFileName());
    assert(bufferIt != m_sounds.end());

    SoundsList& list = bufferIt->second;

    auto removedIt = std::remove_if(list.begin(), list.end(), [sound](SoundPtr ptr) { return sound == ptr.get(); });
    assert(removedIt != list.end());
    assert(std::distance(removedIt, list.end()) == 1);
    list.erase(removedIt, list.end());
}

template<typename TSound, typename TBuffer>
void SoundEngine<TSound,TBuffer>::Reset()
{
    m_buffers.clear();
    for (auto& filePathToSounds : m_sounds)
        assert(filePathToSounds.second.empty());
    m_sounds.clear();
}

template<typename TSound, typename TBuffer>
void SoundEngine<TSound,TBuffer>::AddSound(BufferPtr buffer, SoundPtr sound)
{
    auto jt = m_sounds.find(buffer->GetFileName());
    if (jt != m_sounds.end())
    {
        jt->second.push_back(sound);
    }
    else
    {
        auto insertResultIt = m_sounds.emplace(buffer->GetFileName(), SoundsList());
        assert(insertResultIt.second);
        insertResultIt.first->second.push_back(sound);
    }
}

template<typename TSound, typename TBuffer>
std::shared_ptr<TSound> SoundEngine<TSound,TBuffer>::CreateSound(const std::string& fileName, bool isAutoDelete)
{
    auto it = m_buffers.find(fileName);
    if (it == m_buffers.end())
        m_buffers[fileName] = CreateBuffer(fileName);

    BufferPtr buffer_ptr = m_buffers[fileName];
    SoundPtr sound_ptr(CreateSound(buffer_ptr.get(), isAutoDelete));

    buffer_ptr->AttachSource(sound_ptr.get());

    AddSound(buffer_ptr, sound_ptr);
    return sound_ptr;
}

#endif
