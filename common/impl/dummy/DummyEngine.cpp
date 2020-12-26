#include "DummyEngine.hpp"
#include "DummySound.hpp"
#include "DummyBuffer.hpp"

std::unique_ptr<ISoundEngine> ISoundEngine::Create()
{
    return std::unique_ptr<ISoundEngine>(new DummyEngine());
}

std::shared_ptr<DummySound> DummyEngine::CreateSound(DummyBuffer* buffer, bool isAutoDelete)
{
    return std::shared_ptr<DummySound>(new DummySound(buffer, isAutoDelete), [](DummySound* sound) { delete sound; });
}

std::shared_ptr<DummyBuffer> DummyEngine::CreateBuffer(const std::string& file)
{
    return std::make_shared<DummyBuffer>(file, this);
}
