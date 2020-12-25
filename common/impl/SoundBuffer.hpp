#pragma once

#include "InternalEngine.hpp"
#include "InternalBuffer.hpp"

#include <cstdint>
#include <string>
#include <unordered_set>
#include <memory>
#include <cassert>


template<typename TSound>
class SoundBuffer : public InternalBuffer
{
    using sound_ptr = std::shared_ptr<TSound>;
protected:
    SoundBuffer(const std::string& fileName, InternalEngine* engine)
        : m_fileName(fileName)
        , m_soundEngine(engine)
    {
    }
    
    virtual ~SoundBuffer()
    {
        UnloadAllSources();
    }
    
public:
    
    virtual bool IsActive() const = 0;
    
    float Duration() const  { return m_duration; }
    const std::string& GetFileName() const  { return m_fileName; }
    
    virtual bool LoadMemory() = 0;
    bool CanBeErased() const
    {
        if (!m_sources.empty())
            return false;

        if (!IsActive())
            return false;

        return true;
    }
    
    float GetSizeMem() const { return m_sizeMemory; }

    virtual void AttachSource(TSound* sound)
    {
        m_sources.insert(sound);
        m_soundEngine->OnSourceCreated(this, sound);
    }
    
    virtual void DetachSource(TSound* sound)
    {
        m_sources.erase(sound);
        m_soundEngine->OnSourceRemoved(this, sound);
    }

protected:
    void UnloadAllSources()
    {
        auto it = m_sources.begin();
        while (it != m_sources.end())
        {
            (*it)->Delete();
            it = m_sources.begin();
        }

        assert(m_sources.empty());
    }
private:
    std::unordered_set<TSound*> m_sources;
    std::string m_fileName;

protected:
    InternalEngine* m_soundEngine { nullptr };
    float       m_duration { .0f };
    float       m_sizeMemory { .0f };
};
