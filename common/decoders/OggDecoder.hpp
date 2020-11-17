#pragma once

#include "AudioDecoder.hpp"

class OggDecoder: public AudioDecoder
{
public:
    OggDecoder(const std::string& fileName);
    bool decode() override;
};

