#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>
#include <memory>
#include <cassert>
#include "SoundEngine.hpp" //TODO:: move interal engine to the separate header

//class Sound;
//class SoundEngine;

struct InternalEngine;

struct InternalBuffer
{
    virtual float GetSizeMem() const = 0;
    virtual const std::string& GetFileName() const = 0;

    virtual float Duration() const = 0;
};

template<typename TSound>
class SoundBuffer : public InternalBuffer
{
    //template<typename, typename>
    //friend class SoundEngine;

    using sound_ptr = std::shared_ptr<TSound>;
protected:
    SoundBuffer(const std::string& fileName, InternalEngine* engine);
    virtual ~SoundBuffer();
    
public:
    
    virtual bool IsActive() const = 0;
    
    float Duration() const  { return m_duration; }
    const std::string& GetFileName() const  { return m_fileName; }
    
    virtual bool LoadMemory() = 0;
    bool CanBeErased() const;
    
    float GetSizeMem() const { return m_sizeMemory; }

    virtual void AttachSource(TSound* sound);
    virtual void DetachSource(TSound* sound);

protected:
    void UnloadAllSources();
private:
    std::unordered_set<TSound*> m_sources;
    std::string m_fileName;

protected:
    InternalEngine* m_soundEngine { nullptr };
    float       m_duration { .0f };
    float       m_sizeMemory { .0f };
};

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

