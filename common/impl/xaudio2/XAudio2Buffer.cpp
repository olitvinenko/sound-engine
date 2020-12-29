
#include "XAudio2Sound.hpp"
#include <iostream>
#include "XAudio2Utils.hpp"
#ifdef X2AUDIO_SOUND

#include "XAudio2Buffer.hpp"

#include "../../decoders/AudioDecoder.hpp"


XAudio2Buffer::XAudio2Buffer(const std::string& fileName, InternalEngine* engine)
: SoundBuffer(fileName, engine)
, m_wfEx()
, m_xa2Buffer()
{
    LoadBuffer();
}

bool XAudio2Buffer::LoadBuffer()
{
    auto decoder = AudioDecoder::GetDecoderFor(GetFileName());
    if (!decoder || !decoder->decode())
        return false;

    const char* dataPtr = decoder->GetBuffer().data();
    m_data.assign(dataPtr, dataPtr + decoder->GetBuffer().size());

    m_xa2Buffer =
    {
        XAUDIO2_END_OF_STREAM,
        (UINT32)m_data.size(),
        (const BYTE*)m_data.data(),
        0,
        0,
        0,
        0,
    };

    m_xa2Buffer.pContext = this;

    const std::size_t channelsCount = decoder->GetChannels();
    const std::size_t samplesPerSecond = decoder->GetSampleRate();
 
    m_wfEx.wFormatTag = WAVE_FORMAT_PCM;
    m_wfEx.nChannels = static_cast<WORD>(channelsCount);
    m_wfEx.nSamplesPerSec = static_cast<DWORD>(samplesPerSecond);
    m_wfEx.wBitsPerSample = 16;//decoder->GetBitsPerSample();
    m_wfEx.nBlockAlign = m_wfEx.nChannels * (m_wfEx.wBitsPerSample / 8);
    m_wfEx.nAvgBytesPerSec = m_wfEx.nBlockAlign * static_cast<DWORD>(samplesPerSecond);
    m_wfEx.cbSize = 0; // size of extra information

	return true;
}


void XAudio2Buffer::AttachSource(XAudio2Sound* sound)
{
    IXAudio2SourceVoice* x2Voice = sound->GetSourceVoice();

    if (x2Voice)
    {
        if (!x2WrapCall(x2Voice->SubmitSourceBuffer(&m_xa2Buffer)))
        {
            //TODO::
	        return;
        }
    }

    SoundBuffer<XAudio2Sound>::AttachSource(sound);
}

void XAudio2Buffer::DetachSource(XAudio2Sound* sound)
{
    IXAudio2SourceVoice* x2Voice = sound->GetSourceVoice();

    if (x2Voice)
    {
        if (!SUCCEEDED(x2Voice->FlushSourceBuffers()))
        {
            //TODO::
            //return;
        }
    }

    SoundBuffer<XAudio2Sound>::DetachSource(sound);
}

#endif
