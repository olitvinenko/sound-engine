#include "SoundBuffer.hpp"

#include "Sound.hpp"
#include "SoundEngine.hpp"

#include <cassert>

SoundBuffer::SoundBuffer(const std::string& fileName, SoundEngine* engine)
    : m_fileName(fileName)
    , m_soundEngine(engine)
{
}

SoundBuffer::~SoundBuffer()
{
    UnloadAllSources();
}

bool SoundBuffer::CanBeErased() const
{
    if (!m_sources.empty())
        return false;
    
    if (!IsActive())
        return false;
    
    return true;
}

void SoundBuffer::UnloadAllSources()
{
    auto it = m_sources.begin();
    while (it != m_sources.end())
    {
        (*it)->Delete();
        it = m_sources.begin();
    }
  
    assert(m_sources.empty());
}

//#define SOURCE_OAL 32
//
//ALuint OalBuffer::GrabSource(std::shared_ptr<OalSound> soundOAL)
//{
//    ALuint sourceID(0);
//
//    auto it = m_sources.find(soundOAL);
//    if (m_sources.end() != it)
//    {
//        return (*it)->GetSourceId();
//    }
//
//    const static int MAX_SOURCE = 8;
//    if (m_sources.size() >= MAX_SOURCE)// || CBufferOALManager::Get().GetCounterSource() >= SOURCE_OA)
//    {
//        std::cout << "m_CounterSource >= MAX_SOURCE" << std::endl;
//        //("Error!! m_CounterSource = %i,     g_CounterSource= %i", m_CounterSource, CBufferOALManager::Get().GetCounterSource());
//        return 0;
//    }
//
//    m_soundEngine->ActivateBuffer(shared_from_this());
//
//    if (m_bufferID == 0)
//        return 0;
//
//    ALenum error = AL_NO_ERROR;
//    error = alGetError();
//
//    // grab a source ID from openAL, this will be the base source ID
//    //alGenSources(1, &sourceID);
//    error = alGetError();
//    // attach the buffer to the source
//    alSourcei(soundOAL->GetSourceId(), AL_BUFFER, m_bufferID);
//    error = alGetError();
//
//    if ((error = alGetError()) != AL_NO_ERROR || sourceID == 0)
//    {
//        //("error: %i create sourceID: %i\n", error, sourceID);
//        return 0;
//    }
//    else
//    {
//        auto insertResultIt = m_sources.insert(soundOAL);
//        assert(insertResultIt.second);
//    }
    
    //m_soundEngine->OnSourceCreated(shared_from_this(), soundOAL);
//    return sourceID;
//}

void SoundBuffer::AttachSource(Sound* sound)
{
    m_sources.insert(sound);
    m_soundEngine->OnSourceCreated(this, sound);
}

void SoundBuffer::DetachSource(Sound* sound)
{
    m_sources.erase(sound);
    m_soundEngine->OnSourceRemoved(this, sound);
}
