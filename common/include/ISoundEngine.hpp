#pragma once

#include <string>
#include <memory>

#include "SoundHandle.hpp"

struct ISoundEngine
{
    virtual ~ISoundEngine() = default;
    
    virtual SoundHandle PlaySound(const std::string& filePath, bool isAutoDelete) = 0;
    virtual SoundHandle GetSound(const std::string& filePath, bool isAutoDelete) = 0;
    virtual bool PlayOnce(const std::string& filePath) = 0;
    
    virtual void Update(float deltaTime) = 0;
    
    static std::unique_ptr<ISoundEngine> Create();
    
protected:
    SoundHandle CreateHandle(std::shared_ptr<ISound> sound) { return SoundHandle(sound); }
};
