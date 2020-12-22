#pragma once

#ifdef X2AUDIO_SOUND

#include "../SoundEngine.hpp"

#include <wrl/client.h>

struct IXAudio2MasteringVoice;
struct IXAudio2;
struct IXAudio2Voice;

class XAudio2Engine final : public SoundEngine
{
    struct VoiceDeleter
    {
        void operator()(IXAudio2Voice* voice);
    };

public:
    XAudio2Engine();
    ~XAudio2Engine();

private:
    void Update(float deltaTime) override;
    bool IsValid() const override { return true; }
    
    std::shared_ptr<Sound> CreateSound(SoundBuffer* buffer, bool isAutoDelete) override;
    std::shared_ptr<SoundBuffer> CreateBuffer(const std::string& file) override;

private:
    Microsoft::WRL::ComPtr<IXAudio2> m_xa2;
    std::unique_ptr<IXAudio2MasteringVoice, VoiceDeleter> m_mastering_voice;
};

#endif
