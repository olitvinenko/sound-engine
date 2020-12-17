#include "Mp3Decoder.hpp"
#include "external/mp3/mp3reader.h"

#include <climits>

Mp3Decoder::Mp3Decoder(const std::string& fileName)
 : AudioDecoder(fileName)
{
}

bool Mp3Decoder::decode()
{
    if (m_data.empty())
        return false;

    mp3_callbacks callbacks;
    callbacks.read = AudioDecoder::fileRead;
    callbacks.seek = AudioDecoder::fileSeek;
    callbacks.close = AudioDecoder::fileClose;
    callbacks.tell = AudioDecoder::fileTell;
    

    if (decodeMP3(&callbacks, this, m_buffer, &m_channels, &m_sampleRate, &m_numFrames) == EXIT_SUCCESS
        && m_channels > 0 && m_sampleRate > 0 && m_numFrames > 0)
    {
        m_duration = 1.0f * m_numFrames / m_sampleRate;
        
        std::size_t samplesCount = m_duration * m_sampleRate;
        m_bitsPerSample = (m_buffer.size() / samplesCount / m_channels) * CHAR_BIT;
        
        return true;
    }
    
    return false;
}
