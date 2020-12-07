#ifdef X2AUDIO_SOUND

#include "XAudio2Engine.hpp"
#include "XAudio2Sound.hpp"
#include "XAudio2Buffer.hpp"

std::unique_ptr<ISoundEngine> ISoundEngine::Create()
{
    return std::unique_ptr<ISoundEngine>(new XAudio2Engine());
}

std::shared_ptr<Sound> XAudio2Engine::CreateSound(SoundBuffer* buffer, bool isAutoDelete)
{
    return std::shared_ptr<Sound>(new XAudio2Sound((XAudio2Buffer*)buffer, isAutoDelete), [](XAudio2Sound* sound) { delete sound; });
}

std::shared_ptr<SoundBuffer> XAudio2Engine::CreateBuffer(const std::string& file)
{
    return std::make_shared<XAudio2Buffer>(file, this);
}

#endif
