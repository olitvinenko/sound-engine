#include "DummyBuffer.hpp"

#ifdef SOUND_DUMMY

DummyBuffer::DummyBuffer(const std::string& fileName, InternalEngine* engine)
: SoundBuffer(fileName, engine)
{
    
}

#endif
