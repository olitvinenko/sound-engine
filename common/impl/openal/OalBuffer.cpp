#ifdef OPENAL_SOUND

#include "OalBuffer.hpp"

#include "OalSound.hpp"
#include "OalSoundEngine.hpp"

#include "OalUtils.hpp"

#include "../../decoders/AudioDecoder.hpp"


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
    : SoundBuffer(fileName, engine)
{
    bool isLoaded = LoadBuffer();
    assert(isLoaded);
}

bool OalBuffer::LoadBuffer()
{
    auto decoder = AudioDecoder::GetDecoderFor(GetFileName());
    if(!decoder || !decoder->decode())
        return false;
    
    const ALvoid* bufferData = &decoder->GetBuffer()[0];
    ALint format = GetFormatSound(decoder->GetChannels(), decoder->GetBitsPerSample());
    ALsizei size = (ALsizei)decoder->GetBuffer().size();
    ALsizei freq = decoder->GetSampleRate();
    int duration = decoder->GetDuration();
    

    alCall(alGenBuffers, 1, &m_bufferID);
    if (!alCall(alBufferData, m_bufferID, format, bufferData, size, freq))
    {
        if (m_bufferID && alCall(alIsBuffer, m_bufferID).value)
            alCall(alDeleteBuffers, 1, &m_bufferID);
        
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
    
    if (m_bufferID && alCall(alIsBuffer, m_bufferID).value)
        alCall(alDeleteBuffers, 1, &m_bufferID);
    
    m_bufferID = 0;
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

void OalBuffer::AttachSource(Sound* sound)
{
    OalSound* oalSound = dynamic_cast<OalSound*>(sound); // TODO:: get rid of casting
    ALuint sourceId = oalSound->GetSourceId();

    if (sourceId)
    {
        alCall(alSourcei, sourceId, AL_BUFFER, m_bufferID);
    }
    
    SoundBuffer::AttachSource(sound);
}

void OalBuffer::DetachSource(Sound* sound)
{
    OalSound* oalSound = dynamic_cast<OalSound*>(sound); // TODO:: get rid of casting
    ALuint sourceId = oalSound->GetSourceId();
    
    if (sourceId)
    {
        alCall(alSourcei, sourceId, AL_BUFFER, 0);
    }
    
    SoundBuffer::DetachSource(sound);
}

#endif
