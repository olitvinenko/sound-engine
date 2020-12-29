#ifdef X2AUDIO_SOUND

#include "XAudio2Engine.hpp"
#include "XAudio2Sound.hpp"
#include "XAudio2Buffer.hpp"

#include "XAudio2Utils.hpp"

std::unique_ptr<ISoundEngine> ISoundEngine::Create()
{
    return std::unique_ptr<ISoundEngine>(new XAudio2Engine());
}


void XAudio2Engine::VoiceDeleter::operator()(IXAudio2Voice* voice)
{
    voice->DestroyVoice();
}

XAudio2Engine::XAudio2Engine()
{
    if (!x2Call(XAudio2Create, m_xa2.ReleaseAndGetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR))
        return;

#ifndef NDEBUG
    XAUDIO2_DEBUG_CONFIGURATION config;
    config.TraceMask = ~0U;
    config.BreakMask = 0;
    config.LogThreadID = FALSE;
    config.LogFileline = FALSE;
    config.LogFunctionName = FALSE;
    config.LogTiming = FALSE;
    m_xa2->SetDebugConfiguration(&config);
#endif

    if (!x2Call(CoInitializeEx, nullptr, COINIT_MULTITHREADED))
        return;

    IXAudio2MasteringVoice* mastering_voice = nullptr;

    const auto callResult = x2WrapCall(m_xa2->CreateMasteringVoice(&mastering_voice, 2, 44100));
    if (!callResult)
    {
        CoUninitialize();
        return;
    }

    m_mastering_voice.reset(mastering_voice);
}

XAudio2Engine::~XAudio2Engine()
{
    if (m_mastering_voice)
        CoUninitialize();
}

std::shared_ptr<XAudio2Sound> XAudio2Engine::CreateSound(XAudio2Buffer* buffer, bool isAutoDelete)
{
    return  std::shared_ptr<XAudio2Sound>(new XAudio2Sound(m_xa2.Get(), buffer, isAutoDelete), [](XAudio2Sound* sound) { delete sound; });
}

std::shared_ptr<XAudio2Buffer> XAudio2Engine::CreateBuffer(const std::string& file)
{
    return std::make_shared<XAudio2Buffer>(file, this);
}

#endif
