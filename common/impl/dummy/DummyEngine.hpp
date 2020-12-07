#pragma once

#include "../SoundEngine.hpp"

class DummyEngine final : public SoundEngine
{
private:
    bool IsValid() const override { return true; }
    
    std::shared_ptr<Sound> CreateSound(SoundBuffer* buffer, bool isAutoDelete) override;
    std::shared_ptr<SoundBuffer> CreateBuffer(const std::string& file) override;
};
