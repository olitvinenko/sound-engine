#ifdef SOUND_OPENAL

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

std::shared_ptr<OalSound> OalSoundEngine::CreateSound(OalBuffer* buffer, bool isAutoDelete)
{
    return std::make_shared<OalSound>(buffer, isAutoDelete);
}

std::shared_ptr<OalBuffer> OalSoundEngine::CreateBuffer(const std::string& file)
{
    return std::make_shared<OalBuffer>(file, this);
}

#endif
