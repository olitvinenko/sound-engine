#pragma once

#ifdef DUMMY_SOUND

#include "../SoundEngine.hpp"

class DummyBuffer;
class DummySound;

class DummyEngine final : public SoundEngine<DummySound, DummyBuffer>
{
private:
    bool IsValid() const override { return true; }
    
    std::shared_ptr<DummySound> CreateSound(DummyBuffer* buffer, bool isAutoDelete) override;
    std::shared_ptr<DummyBuffer> CreateBuffer(const std::string& file) override;
};

#endif
