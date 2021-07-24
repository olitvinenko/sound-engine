
#include <iostream>
#include "common/decoders/AudioDecoder.hpp"

#ifdef SOUND_OPENAL

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

#endif

#include "common/include/ISoundEngine.hpp"
#include "common/include/SoundHandle.hpp"
#include "common/include/ISound.hpp"

#include <thread>
#include <chrono>
#include <fstream>

static const char* TEST_SOUND_NAME = "./data/default.ogg"; 

SoundHandle MakeHandle(ISoundEngine* engine)
{
    SoundHandle handle = engine->GetSound(TEST_SOUND_NAME, false);
    handle->SetLoop(true);
    handle->Play();
    return handle;
}

int main()
{
//    auto decoder = AudioDecoder::GetDecoderFor(TEST_SOUND_NAME);
//    if (!decoder->decode())
//    {
//        return EXIT_FAILURE;
//    }
//
//    std::cout << "File = " << decoder->GetFilePath() << std::endl;
//    std::cout << "Duration = " << decoder->GetDuration() << std::endl;
//    std::cout << "Channels = " << decoder->GetChannels() << std::endl;
//    std::cout << "SampleRate = " << decoder->GetSampleRate() << std::endl;
//    std::cout << "BitsPerSample = " << decoder->GetBitsPerSample() << std::endl;
//    std::cout << "NumFrames = " << decoder->GetNumFrames() << std::endl;
//    std::cout << "Buffer.size() = " << decoder->GetBuffer().size() << std::endl;
//
//
//    return EXIT_SUCCESS;


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
        SoundHandle h1 = engine->GetSound(TEST_SOUND_NAME, false);
        h1->SetLoop(true);
        h1->Play();

        //while (1)
        //{
        //    engine->Update(2);
        //}

        //std::this_thread::sleep_for(std::chrono::seconds(40));
        //return 0;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        SoundHandle h2 = engine->GetSound(TEST_SOUND_NAME, false);
        h2->SetLoop(true);
        h2->Play();
        
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            SoundHandle h3 = engine->GetSound(TEST_SOUND_NAME, false);
            h3->SetLoop(true);
            h3->Play();

            {
                {
                    SoundHandle h5 = h3;
                    h3->SetLoop(false);

                    h5 = std::move(h2);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                SoundHandle h4 = engine->GetSound(TEST_SOUND_NAME, false);
                h4->SetLoop(true);
                h4->Play();

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        
        engine->PlayOnce(TEST_SOUND_NAME);

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
        SoundHandle h1 = engine->GetSound(TEST_SOUND_NAME, false);
        h1->SetLoop(true);
        h1->Play();
        
        SoundHandle h2 = engine->GetSound(TEST_SOUND_NAME, false);
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
}
