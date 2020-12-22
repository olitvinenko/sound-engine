
#include "XAudio2Sound.hpp"
#include <iostream>
#ifdef X2AUDIO_SOUND

#include "XAudio2Buffer.hpp"

#include "../../decoders/AudioDecoder.hpp"


XAudio2Buffer::XAudio2Buffer(const std::string& fileName, SoundEngine* engine)
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
        0
    };

    const std::size_t channelsCount = decoder->GetChannels();
    const std::size_t samplesPerSecond = decoder->GetSampleRate();
 
    m_wfEx.wFormatTag = WAVE_FORMAT_PCM;
    m_wfEx.nChannels = channelsCount;
    m_wfEx.nSamplesPerSec = samplesPerSecond;
    m_wfEx.wBitsPerSample = 16;//decoder->GetBitsPerSample();
    m_wfEx.nBlockAlign = m_wfEx.nChannels * (m_wfEx.wBitsPerSample / 8);
    m_wfEx.nAvgBytesPerSec = m_wfEx.nBlockAlign * samplesPerSecond;
    m_wfEx.cbSize = 0; // size of extra information

	return true;
}


void XAudio2Buffer::AttachSource(Sound* sound)
{
    XAudio2Sound* x2Sound = dynamic_cast<XAudio2Sound*>(sound); // TODO::
    IXAudio2SourceVoice* x2Voice = x2Sound->GetSourceVoice();

    if (x2Voice)
    {
        HRESULT res = x2Voice->SubmitSourceBuffer(&m_xa2Buffer);
        if (!SUCCEEDED(res))
        {
	       return;
        }
    }

    SoundBuffer::AttachSource(sound);
}

void XAudio2Buffer::DetachSource(Sound* sound)
{
    XAudio2Sound* x2Sound = dynamic_cast<XAudio2Sound*>(sound); // TODO::
    IXAudio2SourceVoice* x2Voice = x2Sound->GetSourceVoice();

    if (x2Voice)
    {
        HRESULT res = x2Voice->SubmitSourceBuffer(nullptr);
        if (!SUCCEEDED(res)) {}
    }

    SoundBuffer::DetachSource(sound);
}

#endif
