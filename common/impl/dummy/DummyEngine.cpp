#include "DummyEngine.hpp"
#include "DummySound.hpp"
#include "DummyBuffer.hpp"

std::unique_ptr<ISoundEngine> ISoundEngine::Create()
{
    return std::unique_ptr<ISoundEngine>(new DummyEngine());
}

std::shared_ptr<Sound> DummyEngine::CreateSound(SoundBuffer* buffer, bool isAutoDelete)
{
    return std::shared_ptr<Sound>(new DummySound((DummyBuffer*)buffer, isAutoDelete), [](DummySound* sound) { delete sound; });
}

std::shared_ptr<SoundBuffer> DummyEngine::CreateBuffer(const std::string& file)
{
    return std::make_shared<DummyBuffer>(file, this);
}
