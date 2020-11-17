#include "WavDecoder.hpp"
#include "external/wav/tinysndfile.h"

#include <climits>

WavDecoder::WavDecoder(const std::string& fileName)
    :AudioDecoder(fileName)
{
}

void* WavDecoder::onWavOpen(const char* path, void* user)
{
    return user;
}

int WavDecoder::onWavSeek(void* datasource, long offset, int whence)
{
    return AudioDecoder::fileSeek(datasource, (int64_t) offset, whence);
}

int WavDecoder::onWavClose(void* datasource)
{
    return 0;
}

bool WavDecoder::decode()
{
    if (m_data == nullptr)
        return false;
    
    SF_INFO info;
    
    snd_callbacks cb;
    cb.open = onWavOpen;
    cb.read = AudioDecoder::fileRead;
    cb.seek = onWavSeek;
    cb.close = onWavClose;
    cb.tell = AudioDecoder::fileTell;
    
    SNDFILE* handle = NULL;
    bool ret = false;
    do
    {
        handle = sf_open_read(m_fileName.c_str(), &info, &cb, this);
        if (handle == nullptr)
            break;
        
        if (info.frames == 0)
            break;
        
        //LOG("wav info: frames: %d, samplerate: %d, channels: %d, format: %d", info.frames, info.samplerate, info.channels, info.format);
        size_t bufSize = sizeof(short) * info.frames * info.channels;
        unsigned char* buf = (unsigned char*)malloc(bufSize);
        sf_readf_short(handle, (short*)buf, info.frames);
        
        m_buffer.insert(m_buffer.end(), buf, buf + bufSize);
        m_channels = info.channels;
        m_sampleRate = info.samplerate;
        m_numFrames = info.frames;
        m_duration = 1.0f * m_numFrames / m_sampleRate;
        
        std::size_t samplesCount = m_duration * m_sampleRate;
        m_bitsPerSample = (m_buffer.size() / samplesCount / m_channels) * CHAR_BIT;

        free(buf);
        ret = true;
    } while (false);
    
    if (handle != NULL)
        sf_close(handle);
    
    return ret;
}
