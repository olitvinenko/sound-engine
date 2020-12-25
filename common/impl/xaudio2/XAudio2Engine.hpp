#pragma once
#include "XAudio2Sound.hpp"

#ifdef X2AUDIO_SOUND

#include "../SoundEngine.hpp"

#include <wrl/client.h>

struct IXAudio2MasteringVoice;
struct IXAudio2;
struct IXAudio2Voice;

class XAudio2Engine final : public SoundEngine<XAudio2Sound, XAudio2Buffer>
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

    XAudio2Sound* MakeSound(XAudio2Buffer* buffer, bool isAutoDelete) override;

    std::shared_ptr<XAudio2Sound> CreateSound(XAudio2Buffer* buffer, bool isAutoDelete) override;
    std::shared_ptr<XAudio2Buffer> CreateBuffer(const std::string& file) override;

private:
    Microsoft::WRL::ComPtr<IXAudio2> m_xa2;
    std::unique_ptr<IXAudio2MasteringVoice, VoiceDeleter> m_mastering_voice;
};

#endif
