#pragma once

#include <cstddef>
#include <cstdlib>

#include <string>
#include <vector>
#include <memory>

class AudioDecoder
{
protected:
    AudioDecoder(const std::string& fileName);
public:
    static std::unique_ptr<AudioDecoder> GetDecoderFor(const std::string& fileName);
    
    virtual ~AudioDecoder();
    virtual bool decode() = 0;
    
    float GetDuration() const { return m_duration; }
    int GetChannels() const { return m_channels; }
    int GetSampleRate() const { return m_sampleRate; }
    int GetBitsPerSample() const { return m_bitsPerSample; }
    int GetNumFrames() const { return m_numFrames; }
    const std::vector<char>& GetBuffer() const { return m_buffer; }
    
    const std::string& GetFilePath() const { return m_fileName; }

protected:
    static size_t fileRead(void* ptr, size_t size, size_t nmemb, void* datasource);
    static int fileSeek(void* datasource, int64_t offset, int whence);
    static int fileClose(void* datasource);
    static long fileTell(void* datasource);
    
private:
    AudioDecoder(const AudioDecoder&) = delete;
    AudioDecoder(AudioDecoder&&) noexcept = delete;
    AudioDecoder& operator=(const AudioDecoder&) = delete;
    AudioDecoder& operator=(AudioDecoder&&) noexcept = delete;

protected:
    std::vector<char> m_data;
    
    size_t  m_fileCurrPos = 0;
    std::string m_fileName;
    std::vector<char> m_buffer;
    float   m_duration = 0;
    int     m_channels = 0;
    int     m_bitsPerSample = 0;
    int     m_numFrames = 0;
    int     m_sampleRate = 0;
};
