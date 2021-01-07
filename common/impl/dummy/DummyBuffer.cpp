#include "DummyBuffer.hpp"

#ifdef DUMMY_SOUND

DummyBuffer::DummyBuffer(const std::string& fileName, InternalEngine* engine)
: SoundBuffer(fileName, engine)
{
    
}

#endif
