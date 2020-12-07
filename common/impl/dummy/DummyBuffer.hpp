#pragma once

#include "../SoundBuffer.hpp"

class SoundEngine;

class DummyBuffer final : public SoundBuffer
{
public:
    DummyBuffer(const std::string& fileName, SoundEngine* engine);
    
private:
    bool LoadMemory() override { return true; }
    bool IsActive() const override { return true; }
};

