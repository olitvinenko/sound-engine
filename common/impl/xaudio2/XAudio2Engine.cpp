#ifdef X2AUDIO_SOUND

#include "XAudio2Engine.hpp"
#include "XAudio2Sound.hpp"
#include "XAudio2Buffer.hpp"

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
    HRESULT hr;
    hr = XAudio2Create(m_xa2.ReleaseAndGetAddressOf());
    if (FAILED(hr))
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

    hr = CoInitialize(nullptr);
    if (FAILED(hr))
        return;

    IXAudio2MasteringVoice* mastering_voice = nullptr;
    hr = m_xa2->CreateMasteringVoice(&mastering_voice, 2, 44100);

    if (FAILED(hr))
        return;

    m_mastering_voice.reset(mastering_voice);
}

XAudio2Engine::~XAudio2Engine()
{
}

//TODO:: move to sepparate file
#define OPSETID 1U

void XAudio2Engine::Update(float deltaTime)
{
    HRESULT hr = m_xa2->CommitChanges(OPSETID);
    if (FAILED(hr))
    {
	    // ???
    }

    SoundEngine::Update(deltaTime);
}

std::shared_ptr<Sound> XAudio2Engine::CreateSound(SoundBuffer* buffer, bool isAutoDelete)
{
    return std::shared_ptr<Sound>(new XAudio2Sound(m_xa2.Get(), (XAudio2Buffer*)buffer, isAutoDelete), [](XAudio2Sound* sound) { delete sound; });
}

std::shared_ptr<SoundBuffer> XAudio2Engine::CreateBuffer(const std::string& file)
{
    return std::make_shared<XAudio2Buffer>(file, this);
}

#endif
