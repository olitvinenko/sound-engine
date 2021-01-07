#pragma once

#ifdef DUMMY_SOUND

#include "../SoundBuffer.hpp"
#include "DummySound.hpp"

struct InternalEngine;

class DummyBuffer final : public SoundBuffer<DummySound>
{
public:
    DummyBuffer(const std::string& fileName, InternalEngine* engine);
    
private:
    bool LoadMemory() override { return true; }
    bool IsActive() const override { return true; }
};

#endif
