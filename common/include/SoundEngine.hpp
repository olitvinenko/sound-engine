#pragma once

#include <string>
#include <memory>

#include "SoundHandle.hpp"

struct SoundEngine
{
    virtual SoundHandle PlaySound(const std::string& filePath, bool isAutoDelete) = 0;
    virtual SoundHandle PlayOnce(const std::string& filePath) = 0;
    virtual SoundHandle GetSound(const std::string& filePath, bool isAutoDelete) = 0;
    
    virtual void Update(float deltaTime) = 0;
    
    virtual ~SoundEngine() = default;
    
    static std::unique_ptr<SoundEngine> Create();
    
protected:
    SoundHandle MakeHandle(std::shared_ptr<Sound> sound) { return SoundHandle(sound); }
};
