#pragma once

#include "AudioDecoder.hpp"

class Mp3Decoder: public AudioDecoder
{
public:
    Mp3Decoder(const std::string& fileName);
    bool decode() override;
};
