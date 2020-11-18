#pragma once

#include "../../include/SoundEngine.hpp"

#include <OpenAL/OpenAL.h>

#include <unordered_map>
#include <string>
#include <memory>
#include <list>

class OalBuffer;
class OalSound;

class OalSoundEngine :public SoundEngine
{
protected:

public:
    OalSoundEngine();
    ~OalSoundEngine();
    
    SoundHandle PlaySound(const std::string& filePath, bool isAutoDelete) override;
    SoundHandle PlayOnce(const std::string& filePath) override;
    SoundHandle GetSound(const std::string& filePath, bool isAutoDelete) override;
    void Update(float deltaTime) override;
    
    float GetMaxMem() const { return m_maxMem; };
    float GetCurMem() const { return m_curMem; };
    
    bool ActivateBuffer(std::shared_ptr<OalBuffer> buffer);
    bool DeactivateBuffer(std::shared_ptr<OalBuffer> buffer);
    void IncrementMemory(float sizeMem);
    void DecrementMemory(float sizeMem);
    
private:
    void AddSound(std::shared_ptr<OalSound> sound);
    std::shared_ptr<OalSound> CreateSound(const std::string& fileName, bool isAutoDelete);
    
    std::unordered_map<std::string, std::shared_ptr<OalBuffer>> m_buffers;
    
    using SoundList = std::list<std::shared_ptr<OalSound>>;
    std::unordered_map<std::string, SoundList> m_sounds;
    
    ALCdevice* m_device;
    ALCcontext* m_context;

    bool m_initialized;
    
    float m_maxMem;
    float m_curMem;
};
