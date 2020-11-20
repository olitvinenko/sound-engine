#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>

#include <OpenAL/OpenAL.h>

class OalSound;
class OalSoundEngine;

class OalBuffer final
{
    friend class OalSoundEngine;

    using SoundPtr = std::shared_ptr<OalSound>;
public:
    OalBuffer(const std::string& fileName, OalSoundEngine* engine);
    ~OalBuffer();
    
    ALdouble Duration() const { return m_duration; }
    const std::string& GetFileName() const { return m_fileName; }
    
    bool LoadMemory();
    bool CanBeErased() const;
    
    void UnloadAllSources();
    
    float SizeMem() const { return m_sizeMemory; }
    
    void AttachSource(OalSound* sound);
    void DetachSource(OalSound* sound);

private:
    std::unordered_set<OalSound*> m_sources;
    
    ALuint      m_bufferID = 0;
    ALdouble    m_duration = 0;
    std::string m_fileName;
    float       m_sizeMemory = .0f;
    
    OalSoundEngine* m_soundEngine;
};
