#include "OalSoundEngine.hpp"

#include "OalSound.hpp"
#include "OalBuffer.hpp"

#include <algorithm>

std::unique_ptr<SoundEngine> SoundEngine::Create()
{
    return std::unique_ptr<SoundEngine>(new OalSoundEngine());
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
    : m_maxMem(15.f)
    , m_curMem (0.f)
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
    
    m_buffers.clear();
    for (auto& filePathToSounds : m_sounds)
        assert(filePathToSounds.second.empty());
    m_sounds.clear();
    
    m_context.reset();
    m_device.reset();
}

void OalSoundEngine::IncrementMemory(float sizeMem)
{
    m_curMem += sizeMem;
}

void OalSoundEngine::DecrementMemory(float sizeMem)
{
    m_curMem -= sizeMem;
    assert(m_curMem >= 0);
}

void OalSoundEngine::Update(float dt)
{
    auto it = m_sounds.begin();
    while (it != m_sounds.end())
    {
        SoundsList& list = it->second;
        
        for (std::size_t i = 0; i < list.size();)
        {
            if (list[i]->m_isAutoDelete && !list[i]->IsPlaying() && list[i]->m_sourceID) // played once
                list[i]->Delete();
            else
                ++i;
        }
        
        ++it;
    }
}

SoundHandle OalSoundEngine::PlaySound(const std::string& filePath, bool isAutoDelete)
{
    SoundPtr sound = CreateSound(filePath, isAutoDelete);
    sound->Play();
    return MakeHandle(sound);
}

bool OalSoundEngine::PlayOnce(const std::string& filePath)
{
    SoundPtr sound = CreateSound(filePath, true);
    sound->Play();
    return sound->IsPlaying();
}

SoundHandle OalSoundEngine::GetSound(const std::string& filePath, bool isAutoDelete)
{
    std::shared_ptr<OalSound> sound = CreateSound(filePath, isAutoDelete);
    return MakeHandle(sound);
}

std::shared_ptr<OalSound> OalSoundEngine::CreateSound(const std::string& fileName, bool isAutoDelete)
{
    auto it = m_buffers.find(fileName);
    if (it != m_buffers.end())
    {
        SoundPtr sound(new OalSound(it->second.get(), isAutoDelete), [](OalSound* sound) { delete sound; });
        AddSound(it->second, sound);
        return sound;
    }
    else
    {
        auto buffer = std::make_shared<OalBuffer>(fileName, this);
        m_buffers[fileName] = buffer;
        
        SoundPtr sound(new OalSound(buffer.get(), isAutoDelete), [](OalSound* sound) { delete sound; });
        AddSound(buffer, sound);
        return sound;
    }
}

void OalSoundEngine::AddSound(std::shared_ptr<OalBuffer> buffer, std::shared_ptr<OalSound> sound)
{
    auto jt = m_sounds.find(buffer->GetFileName());
    if (jt != m_sounds.end())
    {
        jt->second.push_back(sound);
    }
    else
    {
        auto insertResultIt = m_sounds.emplace(buffer->GetFileName(), SoundsList());
        assert(insertResultIt.second);
        insertResultIt.first->second.push_back(sound);
    }
}

bool OalSoundEngine::DeactivateBuffer(std::shared_ptr<OalBuffer> buffer)
{
    DecrementMemory(buffer->m_sizeMemory);
    return true;
}

void OalSoundEngine::OnSourceCreated(OalBuffer* buffer, OalSound* sound)
{
    IncrementMemory(buffer->m_sizeMemory);
    //TODO::
}

void OalSoundEngine::OnSourceRemoved(OalBuffer* buffer, OalSound* sound)
{
    assert(!m_sounds.empty());
    
    auto bufferIt = m_sounds.find(buffer->GetFileName());
    assert(bufferIt != m_sounds.end());
    
    SoundsList& list = bufferIt->second;
    
    auto removedIt = std::remove_if(list.begin(), list.end(), [sound](SoundPtr ptr) { return sound == ptr.get(); });
    assert(removedIt != list.end());
    assert(std::distance(removedIt, list.end()) == 1);
    list.erase(removedIt, list.end());
}

bool OalSoundEngine::ActivateBuffer(std::shared_ptr<OalBuffer> buffer)
{
    if (buffer->m_bufferID)
        return true;
    
    if (!buffer->LoadMemory())
        return false;
    
    if (buffer->SizeMem() >= GetMaxMem()) // too big file
    {
        buffer->UnloadAllSources();
        
#ifndef NDEBUG
        assert(false);
#endif
        return false;
    }
    
    IncrementMemory(buffer->SizeMem());
    
    if (GetCurMem() >= GetMaxMem())
    {
        for (auto it = m_buffers.begin(); it != m_buffers.end(); ++it)
        {
            auto buffer = it->second;
            auto& sources = buffer->m_sources;
            
            bool anyPlaying = std::any_of(sources.begin(), sources.end(), [](OalSound* it)
            {
                return it->IsPlaying();
            });
            
            if (anyPlaying || !buffer->CanBeErased())
                continue;
            
            buffer->UnloadAllSources();
        }
    }
    
    
    if (GetCurMem() >= GetMaxMem())
    {
#ifndef NDEBUG
        assert(false);
#endif
        return false;
    }
    
    return true;
}
