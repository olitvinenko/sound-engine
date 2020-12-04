#pragma once

#include "AudioDecoder.hpp"

class OggDecoder final : public AudioDecoder
{
public:
    OggDecoder(const std::string& fileName);
protected:
    bool decode() override;
};

