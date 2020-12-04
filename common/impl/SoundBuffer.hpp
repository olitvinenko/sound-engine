#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>

class Sound;
class SoundEngine;

class SoundBuffer
{
    friend class SoundEngine;

    using sound_ptr = std::shared_ptr<Sound>;
protected:
    SoundBuffer(const std::string& fileName, SoundEngine* engine);
    ~SoundBuffer();
    
public:
    
    virtual bool IsActive() const = 0;
    
    float Duration() const { return m_duration; }
    const std::string& GetFileName() const { return m_fileName; }
    
    virtual bool LoadMemory() = 0;
    bool CanBeErased() const;
    
    float SizeMem() const { return m_sizeMemory; }

//protected:
    virtual void AttachSource(Sound* sound);
    virtual void DetachSource(Sound* sound);

protected:
    void UnloadAllSources();
private:
    std::unordered_set<Sound*> m_sources;
    std::string m_fileName;

protected:
    SoundEngine* m_soundEngine { nullptr };
    float       m_duration { .0f };
    float       m_sizeMemory { .0f };
};
