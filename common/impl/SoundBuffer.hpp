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

#include "SoundBuffer.cpp"
