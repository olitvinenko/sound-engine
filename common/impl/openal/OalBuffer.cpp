#include "OalBuffer.hpp"

#include "OalSound.hpp"
#include "OalSoundEngine.hpp"

#include "../../decoders/AudioDecoder.hpp"

#include <fstream>
#include <iostream>

static ALint GetFormatSound(ALint channels, ALint bitsPerSample)
{
    ALint format=AL_FORMAT_MONO8;
    if ( channels == 2 ) // stereo
    {
        if ( bitsPerSample == 16 )
            format = AL_FORMAT_STEREO16;
        else
            format = AL_FORMAT_STEREO8;
    }
    else // mono
    {
        if ( bitsPerSample == 16 )
            format = AL_FORMAT_MONO16;
        else
            format = AL_FORMAT_MONO8;
    }
    return format;
}

OalBuffer::OalBuffer(const std::string& fileName, OalSoundEngine* engine)
    : m_fileName(fileName)
    , m_soundEngine(engine)
{
    bool memoryLoaded = LoadMemory();
    assert(memoryLoaded);
}

bool OalBuffer::LoadMemory()
{
    auto decoder = AudioDecoder::GetDecoderFor(m_fileName);
    if(!decoder || !decoder->decode())
        return false;
    
    const ALvoid* bufferData = &decoder->GetBuffer()[0];
    ALint format = GetFormatSound(decoder->GetChannels(), decoder->GetBitsPerSample());
    ALsizei size = (ALsizei)decoder->GetBuffer().size();
    ALsizei freq = decoder->GetSampleRate();
    int duration = decoder->GetDuration();
    
    alGenBuffers(1, &m_bufferID);
    alBufferData(m_bufferID, format, bufferData, size, freq);
    
    if (alGetError() != AL_NO_ERROR)
    {
        if (m_bufferID && alIsBuffer(m_bufferID))
            alDeleteBuffers(1, &m_bufferID);
        
        m_bufferID = 0;
        return false;
    }
    
    m_duration = duration;
    m_sizeMemory = (float)size / (1024.0f * 1024.0f);
    
    return true;
}

OalBuffer::~OalBuffer()
{
    UnloadAllSources();
    
    if (m_bufferID && alIsBuffer(m_bufferID))
        alDeleteBuffers(1, &m_bufferID);
    
    m_bufferID = 0;
}

bool OalBuffer::CanBeErased() const
{
    if (!m_sources.empty())
        return false;
    
    if (m_bufferID == 0) // already unloaded
        return false;
    
    return true;
}

void OalBuffer::UnloadAllSources()
{
    auto it = m_sources.begin();
    while (it != m_sources.end())
    {
        (*it)->Delete();
        it = m_sources.begin();
    }
  
    assert(m_sources.empty());

//    m_soundEngine->DeactivateBuffer(shared_from_this());
//    m_bufferID = 0;
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
//        //ALOG("error: %i create sourceID: %i\n", error, sourceID);
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

void OalBuffer::AttachSource(SoundPtr sound)
{
    alGetError(); //TODO::
    alSourcei(sound->GetSourceId(), AL_BUFFER, m_bufferID);
    alGetError(); //TODO::
    
    m_sources.insert(sound);
    m_soundEngine->OnSourceCreated(shared_from_this(), sound);
}

void OalBuffer::DetachSource(SoundPtr sound)
{
    alGetError(); //TODO::
    alSourcei(sound->GetSourceId(), AL_BUFFER, 0);
    alGetError(); //TODO::
    
    m_sources.erase(sound);
    m_soundEngine->OnSourceRemoved(shared_from_this(), sound);
}
