#include "OalSoundEngine.hpp"

#include "OalSound.hpp"
#include "OalBuffer.hpp"

#include <algorithm>

std::unique_ptr<ISoundEngine> ISoundEngine::Create()
{
    return std::unique_ptr<ISoundEngine>(new OalSoundEngine());
}


void OalSoundEngine::OalContextDeleter::operator()(ALCcontext* context)
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
}

void OalSoundEngine::OalDeviceDeleter::operator()(ALCdevice* device)
{
    alcCloseDevice(device);
}

static bool CheckOpenAlError(ALCdevice* device)
{
    ALenum error = alcGetError(device);
    if (error != ALC_NO_ERROR)
    {
        printf("\nALC Error %x occurred: %s\n", error, alcGetString(device, error));
        
        return true;
    }
    
    return false;;
}

OalSoundEngine::OalSoundEngine()
    : SoundEngine()
{
    ALCdevice* device = alcOpenDevice(NULL);
    if (!device)
        return;
    
    m_device.reset(device);
    assert(!CheckOpenAlError(m_device.get()));
       
    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context)
    {
        m_device.reset();
        return;
    }
    
    m_context.reset(context);
    assert(!CheckOpenAlError(m_device.get()));

    alcMakeContextCurrent(m_context.get());
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
