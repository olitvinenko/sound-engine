#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

#include "InternalBuffer.hpp"
#include "InternalEngine.hpp"

class SoundHandle;

template<typename TSound, typename TBuffer>
class SoundEngine : public InternalEngine
{
private:
    using SoundPtr = std::shared_ptr<TSound>;
    using BufferPtr = std::shared_ptr<TBuffer>;
    using SoundsList = std::vector<SoundPtr>;

public:
    SoundEngine();
    ~SoundEngine();
    
private:
    SoundHandle PlaySound(const std::string& filePath, bool isAutoDelete) override final;
    SoundHandle GetSound(const std::string& filePath, bool isAutoDelete) override final;
    bool PlayOnce(const std::string& filePath) override final;
protected:
    void Update(float deltaTime) override;
    
private:
    float GetMaxMem() const { return m_maxMem; };
    float GetCurMem() const { return m_curMem; };
    
    bool ActivateBuffer(BufferPtr buffer);
    bool DeactivateBuffer(BufferPtr buffer);
    
    void IncrementMemory(float sizeMem);
    void DecrementMemory(float sizeMem);
    
public:
    void OnSourceCreated(InternalBuffer* buffer, InternalSound* sound) override;
    void OnSourceRemoved(InternalBuffer* buffer, InternalSound* sound) override;

protected:
    virtual std::shared_ptr<TSound> CreateSound(TBuffer* buffer, bool isAutoDelete) = 0;
    virtual std::shared_ptr<TBuffer> CreateBuffer(const std::string& file) = 0;
    
    virtual bool IsValid() const = 0;
    
    void Reset();
    
private:
    void AddSound(BufferPtr buffer, SoundPtr sound);
    SoundPtr CreateSound(const std::string& fileName, bool isAutoDelete);

    // loaded buffer => all created sources from it
    std::unordered_map<std::string, SoundsList> m_sounds;
    // file path => loaded buffer
    std::unordered_map<std::string, BufferPtr> m_buffers;
    
    float m_maxMem;
    float m_curMem;
};

#include "SoundEngine.cpp"
