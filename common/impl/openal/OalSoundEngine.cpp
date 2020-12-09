#ifdef OPENAL_SOUND

#include "OalSoundEngine.hpp"

#include "OalSound.hpp"
#include "OalBuffer.hpp"

#include "OalUtils.hpp"

#include <algorithm>

std::unique_ptr<ISoundEngine> ISoundEngine::Create()
{
    return std::unique_ptr<ISoundEngine>(new OalSoundEngine());
}


void OalSoundEngine::OalContextDeleter::operator()(ALCcontext* context)
{
    alCall(alcMakeContextCurrent, (ALCcontext*)NULL);
    alCall(alcDestroyContext, context);
}

void OalSoundEngine::OalDeviceDeleter::operator()(ALCdevice* device)
{
    alCall(alcCloseDevice, device);
}

OalSoundEngine::OalSoundEngine()
    : SoundEngine()
{
    ALCdevice* device = alCall(alcOpenDevice, (const ALCchar *)NULL);
    if (!device)
        return;
    
    m_device.reset(device);

    ALCcontext* context = alCall(alcCreateContext, device, (const ALCint*)NULL);
    if (!context)
    {
        m_device.reset();
        return;
    }
    
    m_context.reset(context);
    alCall(alcMakeContextCurrent, m_context.get());
}

OalSoundEngine::~OalSoundEngine()
{
    if (!m_device || !m_context)
        return;
    
    SoundEngine::Reset();
    
    m_context.reset();
    m_device.reset();
}

std::shared_ptr<Sound> OalSoundEngine::CreateSound(SoundBuffer* buffer, bool isAutoDelete)
{
    return std::shared_ptr<Sound>(new OalSound((OalBuffer*)buffer, isAutoDelete), [](OalSound* sound) { delete sound; });
}

std::shared_ptr<SoundBuffer> OalSoundEngine::CreateBuffer(const std::string& file)
{
    return std::make_shared<OalBuffer>(file, this);
}

#endif
