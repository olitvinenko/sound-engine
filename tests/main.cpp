
#include <iostream>
#include "common/decoders/AudioDecoder.hpp"

#include <OpenAL/OpenAL.h>

void printInfo(const AudioDecoder* decoder)
{
    std::cout << "File = " << decoder->GetFilePath() << std::endl;
    std::cout << "Duration = " << decoder->GetDuration() << std::endl;
    std::cout << "Channels = " << decoder->GetChannels() << std::endl;
    std::cout << "SampleRate = " << decoder->GetSampleRate() << std::endl;
    std::cout << "BitsPerSample = " << decoder->GetBitsPerSample() << std::endl;
    std::cout << "NumFrames = " << decoder->GetNumFrames() << std::endl;
    std::cout << "Buffer.size() = " << decoder->GetBuffer().size() << std::endl;
}

void testFile(const char* filePath)
{
    auto decoder = AudioDecoder::GetDecoderFor(filePath);
    if (!decoder)
    {
        std::cerr << "Unsupported audio file extension " << filePath << std::endl;
        return;
    }
        
    if (!decoder->decode())
    {
        std::cerr << "Can not decode audio file " << filePath << std::endl;
        return;
    }
    
    printInfo(decoder.get());
}

#include "common/include/ISoundEngine.hpp"
#include "common/include/SoundHandle.hpp"
#include "common/include/ISound.hpp"

#include <thread>
#include <chrono>

SoundHandle MakeHandle(ISoundEngine* engine)
{
    SoundHandle handle = engine->GetSound("Retrigger.mp3", false);
    handle->SetLoop(true);
    handle->Play();
    return handle;
}

int main()
{
//    testFile("free_game_music_advanced.mp3");std::cout << std::endl;
//    testFile("file_example_WAV_2MG.wav");std::cout << std::endl;
//    testFile("default.ogg");
//
    std::unique_ptr<ISoundEngine> engine = ISoundEngine::Create();
    
//    SoundHandle handle = engine->GetSound("Retrigger.mp3", false);
//    handle->SetLoop(true);
//    handle->Play();
//
//    SoundHandle sss(std::move(MakeHandle(engine.get())));
//
//    if (handle)
//    {
//        SoundHandle handle2 = std::move(handle);
//        assert(!handle);
//        handle2->Stop();
//    }
    
    
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    
    {
        SoundHandle h1 = engine->GetSound("Retrigger.mp3", false);
        h1->SetLoop(true);
        h1->Play();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        SoundHandle h2 = engine->GetSound("Retrigger.mp3", false);
        h2->SetLoop(true);
        h2->Play();
        
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            SoundHandle h3 = engine->GetSound("Retrigger.mp3", false);
            h3->SetLoop(true);
            h3->Play();

            {
                {
                    SoundHandle h5 = h3;
                    h3->SetLoop(false);

                    h5 = std::move(h2);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                SoundHandle h4 = engine->GetSound("Retrigger.mp3", false);
                h4->SetLoop(true);
                h4->Play();

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        
        engine->PlayOnce("Retrigger.mp3");

        int c = 0;
        while (h1)
        {
            engine->Update(0.1);

            if (c++ >= 50)
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << h1->GetTime();
    }
    
    {
        SoundHandle h1 = engine->GetSound("Retrigger.mp3", false);
        h1->SetLoop(true);
        h1->Play();
        
        SoundHandle h2 = engine->GetSound("Retrigger.mp3", false);
        h2->SetLoop(true);
        h2->Play();
        
        int c = 0;
        while (h1)
        {
            engine->Update(0.1);


            if (c++ >= 50)
                break;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        engine.reset();
    }
    
    
//    ALuint buffer, source;
//    ALint state;
//    ALsizei size;
//    ALsizei frequency;
//    ALenum format;
//
//    auto decoder = AudioDecoder::GetDecoderFor("free_game_music_advanced.mp3");
//    if (!decoder)
//    {
//        std::cerr << "Unsupported audio file extension " << std::endl;
//        return -1;
//    }
//
//    if (!decoder->decode())
//    {
//        std::cerr << "Can not decode audio file " << std::endl;
//        return -1;
//    }
//
//    size = decoder->GetBuffer().size();
//    frequency = decoder->GetSampleRate();
//
//    if (decoder->GetChannels() == 1)
//    {
//        if (decoder->GetBitsPerSample() == 8)
//            format = AL_FORMAT_MONO8;
//        else if (decoder->GetBitsPerSample() == 16)
//            format = AL_FORMAT_MONO16;
//    }
//    else if (decoder->GetChannels() == 2)
//    {
//        if (decoder->GetBitsPerSample() == 8)
//            format = AL_FORMAT_STEREO8;
//        else if (decoder->GetBitsPerSample() == 16)
//            format = AL_FORMAT_STEREO16;
//    }
//
//    format = AL_FORMAT_MONO16;
//
//    ALCcontext *context;
//    ALCdevice *device;
//
//    device = alcOpenDevice(nullptr);
//    if (device == NULL)
//    {
//        std::cerr << "Error finding default Audio Output Device" << std::endl;
//    }
//
//    context = alcCreateContext(device,NULL);
//
//    alcMakeContextCurrent(context);
//
//    alGenBuffers(1, &buffer);
//    if(alGetError() != AL_NO_ERROR) {
//        std::cerr << alGetError() << std::endl;
//    }
//    alBufferData(buffer, format, (void*)decoder->GetBuffer().data(),
//                         size, frequency);
//    if(alGetError() != AL_NO_ERROR) {
//        std::cerr << alGetError() << std::endl;
//    }
//
//    alGenSources(1, &source);
//    alSourcei(source, AL_BUFFER, buffer);
//
//    // Play
//    alSourcePlay(source);
//
//    // Wait for the song to complete
//    do {
//        alGetSourcei(source, AL_SOURCE_STATE, &state);
//    } while (state == AL_PLAYING);
//
//    // Clean up sources and buffers
//    alDeleteSources(1, &source);
//    alDeleteBuffers(1, &buffer);
}
