#include "AudioDecoder.hpp"
#include "Mp3Decoder.hpp"
#include "WavDecoder.hpp"
#include "OggDecoder.hpp"

#include <fstream>
#include <algorithm>

size_t AudioDecoder::fileRead(void* ptr, size_t size, size_t nmemb, void* datasource)
{
    AudioDecoder* decoder = static_cast<AudioDecoder*>(datasource);
    const size_t toReadBytes = std::min(static_cast<size_t>(decoder->m_size - decoder->m_fileCurrPos), static_cast<size_t>(nmemb * size));
    if (toReadBytes > 0)
    {
        std::memcpy(ptr, decoder->m_data + decoder->m_fileCurrPos, toReadBytes);
        decoder->m_fileCurrPos += toReadBytes;
    }

    return toReadBytes;
}

int AudioDecoder::fileSeek(void* datasource, int64_t offset, int whence)
{
    AudioDecoder* decoder = static_cast<AudioDecoder*>(datasource);
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
    AudioDecoder* decoder = static_cast<AudioDecoder*>(datasource);
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
    if(extension == "wav")
        return std::unique_ptr<AudioDecoder>(new WavDecoder(fileName));
    if(extension == "ogg")
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

    m_data = new (std::nothrow) char[m_size];
    std::memset(m_data, 0, m_size);
    if (!m_data)
    {
	    // TODO::
        fs.close();
        return;
    }

    fs.seekg(0, std::ios::beg);
    fs.read(m_data, m_size);
    
    fs.close();
}

AudioDecoder::~AudioDecoder()
{
	delete[] m_data;
}
