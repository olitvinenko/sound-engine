#pragma once

#include <string>

struct InternalBuffer
{
    virtual float GetSizeMem() const = 0;
    virtual const std::string& GetFileName() const = 0;

    virtual float Duration() const = 0;
};
