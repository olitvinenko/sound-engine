#pragma once

#include "AudioDecoder.hpp"

class Mp3Decoder final : public AudioDecoder
{
public:
    Mp3Decoder(const std::string& fileName);
protected:
    bool decode() override;
};
