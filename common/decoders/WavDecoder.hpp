#pragma once

#include "AudioDecoder.hpp"

class WavDecoder: public AudioDecoder
{
public:
    WavDecoder(const std::string& fileName);
    bool decode() override;
    
protected:
    static void* onWavOpen(const char* path, void* user);
    static int onWavSeek(void* datasource, long offset, int whence);
    static int onWavClose(void* datasource);
};

