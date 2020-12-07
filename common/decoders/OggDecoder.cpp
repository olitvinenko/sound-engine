#include "OggDecoder.hpp"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include <exception>
#include <climits>
#include <stdexcept>

OggDecoder::OggDecoder(const std::string& fileName)
    :AudioDecoder(fileName)
{
}

bool OggDecoder::decode()
{
    if (m_data == nullptr)
        return false;
    
    ov_callbacks callbacks;
    callbacks.read_func = AudioDecoder::fileRead;
    callbacks.seek_func = AudioDecoder::fileSeek;
    callbacks.close_func = AudioDecoder::fileClose;
    callbacks.tell_func = AudioDecoder::fileTell;
    
    OggVorbis_File vf;
    if( int result = ov_open_callbacks(this, &vf, nullptr, 0, callbacks) )
    {
        switch( result )
        {
        case OV_EREAD: throw std::runtime_error("A read from media returned an error");
        case OV_ENOTVORBIS: throw std::runtime_error("Bitstream does not contain any Vorbis data");
        case OV_EVERSION: throw std::runtime_error("Vorbis version mismatch");
        case OV_EBADHEADER: throw std::runtime_error("Invalid Vorbis bitstream header");
        case OV_EFAULT: throw std::runtime_error("Internal logic fault; indicates a bug or heap/stack corruption");
        }
        throw std::runtime_error("unknown error opening ov stream");
    }
    
    const char WORD_SIZE = 2;

    try
    {
        vorbis_info *pinfo = ov_info(&vf, -1);
        if( nullptr == pinfo )
        {
            throw std::runtime_error("could not get info from ov stream");
        }
        
        m_channels = pinfo->channels;
        m_sampleRate = pinfo->rate;
        m_bitsPerSample = WORD_SIZE * CHAR_BIT;
        m_duration = static_cast<float>(ov_time_total(&vf, -1));

        ogg_int64_t nSamples = ov_pcm_total(&vf, -1);
        size_t size = static_cast<size_t>(nSamples * pinfo->channels * WORD_SIZE);
        m_buffer.resize(size);
        
        m_numFrames = nSamples / m_channels;

        int bitstream = 0;
        size_t total = 0;

        while (total < size)
        {
            long ret = ov_read(&vf, &m_buffer[total], size - total, 0, WORD_SIZE, 1, &bitstream);
            if( 0 == ret )
            {
                break; // eof
            }
            if( ret < 0 )
            {
                // error in stream
                switch( ret )
                {
                case OV_HOLE: throw std::runtime_error("garbage between pages, loss of sync followed by recapture, or a corrupt page");
                case OV_EBADLINK: throw std::runtime_error("invalid stream section or the requested link is corrupt");
                case OV_EINVAL: throw std::runtime_error("initial file headers couldn't be read or are corrupt");
                }
                throw std::runtime_error("unknown error in ov stream");
            }
            else
            {
                total += ret;
            }
        }
        
        std::size_t samplesCount = m_duration * m_sampleRate;
        m_bitsPerSample = (m_buffer.size() / samplesCount / m_channels) * CHAR_BIT;
    }
    catch (const std::exception&)
    {
        ov_clear(&vf);
        return false;
    }
    
    ov_clear(&vf);
    return true;
}
