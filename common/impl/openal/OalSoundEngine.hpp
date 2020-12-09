#pragma once

#ifdef OPENAL_SOUND

#include "../SoundEngine.hpp"

#include <OpenAL/OpenAL.h>

#include <string>
#include <memory>

class OalBuffer;
class OalSound;

class OalSoundEngine final : public SoundEngine
{
private:
    struct OalContextDeleter { void operator()(ALCcontext* context); };
    struct OalDeviceDeleter { void operator()(ALCdevice* device); };

public:
    OalSoundEngine();
    ~OalSoundEngine();
    
private:
    bool IsValid() const override { return m_device && m_context; }
    
    std::shared_ptr<Sound> CreateSound(SoundBuffer* buffer, bool isAutoDelete) override;
    std::shared_ptr<SoundBuffer> CreateBuffer(const std::string& file) override;

private:
    std::unique_ptr<ALCdevice, OalDeviceDeleter> m_device;
    std::unique_ptr<ALCcontext, OalContextDeleter> m_context;
};

#endif
