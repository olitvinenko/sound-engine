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
    return fileSeek(datasource, static_cast<int64_t>(offset), whence);
}


bool WavDecoder::decode()
{
    if (m_data.empty())
        return false;
    
    SF_INFO info;
    
    snd_callbacks cb;
    cb.open = onWavOpen;
    cb.read = fileRead;
    cb.seek = onWavSeek;
    cb.close = fileClose;
    cb.tell = fileTell;

    auto handleDeleter = [](SNDFILE* hnd)
    {
        if (hnd)
			sf_close(hnd);
    };
    std::unique_ptr<SNDFILE, decltype(handleDeleter)> handle(sf_open_read(m_fileName.c_str(), &info, &cb, this), handleDeleter);

    if (!handle)
        return false;;
    
    if (info.frames == 0)
        return false;

    unsigned char* buf = nullptr;

	const size_t bufSize = sizeof(short) * info.frames * info.channels;
    
    buf = static_cast<unsigned char*>(malloc(bufSize));
    if (!buf)
        return false;

    sf_readf_short(handle.get(), reinterpret_cast<short*>(buf), info.frames);

    m_buffer.insert(m_buffer.end(), buf, buf + bufSize);
    m_channels = info.channels;
    m_sampleRate = info.samplerate;
    m_numFrames = info.frames;
    m_duration = 1.0f * m_numFrames / m_sampleRate;

    const std::size_t samplesCount = m_duration * m_sampleRate;
    m_bitsPerSample = (m_buffer.size() / samplesCount / m_channels) * CHAR_BIT;

    free(buf);
    return true;
}
