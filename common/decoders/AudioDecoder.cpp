#include "AudioDecoder.hpp"
#include "Mp3Decoder.hpp"
#include "WavDecoder.hpp"
#include "OggDecoder.hpp"

#include <fstream>
#include <algorithm>

size_t AudioDecoder::fileRead(void* ptr, size_t size, size_t nmemb, void* datasource)
{
    AudioDecoder* decoder = (AudioDecoder*)datasource;
    size_t toReadBytes = std::min((size_t)(decoder->m_size - decoder->m_fileCurrPos), (size_t)(nmemb * size));
    if (toReadBytes > 0)
    {
        memcpy(ptr, (unsigned char*) decoder->m_data + decoder->m_fileCurrPos, toReadBytes);
        decoder->m_fileCurrPos += toReadBytes;
    }

    return toReadBytes;
}

int AudioDecoder::fileSeek(void* datasource, int64_t offset, int whence)
{
    AudioDecoder* decoder = (AudioDecoder*)datasource;
    if (whence == SEEK_SET)
        decoder->m_fileCurrPos = offset;
    else if (whence == SEEK_CUR)
        decoder->m_fileCurrPos = decoder->m_fileCurrPos + offset;
    else if (whence == SEEK_END)
        decoder->m_fileCurrPos = decoder->m_size;
    return 0;
}

int AudioDecoder::fileClose(void* datasource)
{
    return 0;
}

long AudioDecoder::fileTell(void* datasource)
{
    AudioDecoder* decoder = (AudioDecoder*)datasource;
    return (long) decoder->m_fileCurrPos;
}

std::unique_ptr<AudioDecoder> AudioDecoder::GetDecoderFor(const std::string& fileName)
{
    std::string path(fileName);
    
    std::string::size_type idx;
    idx = path.rfind('.');
    if(idx == std::string::npos)
        return nullptr;
    
    std::string extension = path.substr(idx + 1);
    
    if(extension == "mp3")
        return std::unique_ptr<AudioDecoder>(new Mp3Decoder(fileName));
    else if(extension == "wav")
        return std::unique_ptr<AudioDecoder>(new WavDecoder(fileName));
    else if(extension == "ogg")
        return std::unique_ptr<AudioDecoder>(new OggDecoder(fileName));
    
    return nullptr;
}

AudioDecoder::AudioDecoder(const std::string& fileName)
    : m_fileName(fileName)
{
    std::fstream fs(fileName);
    if (!fs.is_open())
        return;
    
    fs.seekg(0, std::ios::end);
    
    m_size = fs.tellg();
    m_data = malloc(m_size);
    if (!m_data)
    {
        fs.close();
        return; // throw std::bad_alloc();
    }
    
    fs.seekg(0, std::ios::beg);
    fs.read((char*)m_data, m_size);
    
    fs.close();
}

AudioDecoder::~AudioDecoder()
{
    if (m_data)
        free(m_data);
}
