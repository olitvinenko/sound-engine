#pragma once

#include "../include/ISoundEngine.hpp"

struct InternalBuffer;
class InternalSound;

struct InternalEngine : public ISoundEngine
{
    virtual void OnSourceCreated(InternalBuffer* buffer, InternalSound* sound) = 0;
    virtual void OnSourceRemoved(InternalBuffer* buffer, InternalSound* sound) = 0;
};
