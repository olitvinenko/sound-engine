#pragma once

#include "../../include/SoundEngine.hpp"

#include <OpenAL/OpenAL.h>

#include <unordered_map>
#include <string>
#include <memory>
#include <list>
#include <vector>
#include <unordered_set>

class OalBuffer;
class OalSound;

class OalSoundEngine : public SoundEngine
{
private:
    struct OalContextDeleter
    {
        void operator()(ALCcontext* context);
    };

    struct OalDeviceDeleter
    {
        void operator()(ALCdevice* device);
    };
    
    using SoundPtr = std::shared_ptr<OalSound>;
    using BufferPtr = std::shared_ptr<OalBuffer>;

    using SoundsList = std::vector<SoundPtr>;

public:
    OalSoundEngine();
    ~OalSoundEngine();
    
    SoundHandle PlaySound(const std::string& filePath, bool isAutoDelete) override;
    SoundHandle GetSound(const std::string& filePath, bool isAutoDelete) override;
    bool PlayOnce(const std::string& filePath) override;
    void Update(float deltaTime) override;
    
    float GetMaxMem() const { return m_maxMem; };
    float GetCurMem() const { return m_curMem; };
    
    bool ActivateBuffer(BufferPtr buffer);
    bool DeactivateBuffer(BufferPtr buffer);
    
    void IncrementMemory(float sizeMem);
    void DecrementMemory(float sizeMem);
    
    void OnSourceCreated(OalBuffer* buffer, OalSound* sound);
    void OnSourceRemoved(OalBuffer* buffer, OalSound* sound);
    
private:
    void AddSound(BufferPtr buffer, SoundPtr sound);
    SoundPtr CreateSound(const std::string& fileName, bool isAutoDelete);

    std::unique_ptr<ALCdevice, OalDeviceDeleter> m_device;
    std::unique_ptr<ALCcontext, OalContextDeleter> m_context;
    
    // loaded buffer => all created sources from it
    std::unordered_map<std::string, SoundsList> m_sounds;
    // file path => loaded buffer
    std::unordered_map<std::string, BufferPtr> m_buffers;
    
    float m_maxMem;
    float m_curMem;
};
