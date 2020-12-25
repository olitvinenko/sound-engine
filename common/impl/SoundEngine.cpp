
#include "SoundEngine.hpp"

#include "Sound.hpp"
#include "SoundBuffer.hpp"
#include "../include/SoundHandle.hpp"
#include <algorithm>
#include <cassert>


//SoundEngine::SoundEngine()
//    : m_maxMem(15.f)
//    , m_curMem (0.f)
//{
//
//}
//
//SoundEngine::~SoundEngine()
//{
//    Reset();
//}
//
//void SoundEngine::Reset()
//{
//    m_buffers.clear();
//    for (auto& filePathToSounds : m_sounds)
//        assert(filePathToSounds.second.empty());
//    m_sounds.clear();
//}
//
//void SoundEngine::IncrementMemory(float sizeMem)
//{
//    m_curMem += sizeMem;
//}
//
//void SoundEngine::DecrementMemory(float sizeMem)
//{
//    m_curMem -= sizeMem;
//    assert(m_curMem >= 0);
//}
//
//void SoundEngine::Update(float dt)
//{
//    auto it = m_sounds.begin();
//    while (it != m_sounds.end())
//    {
//        SoundsList& list = it->second;
//        
//        for (std::size_t i = 0; i < list.size();)
//        {
//            if (list[i]->m_isAutoDelete && !list[i]->IsPlaying() && list[i]->IsValid()) // played once
//                list[i]->Delete();
//            else
//                ++i;
//        }
//        
//        ++it;
//    }
//}
//
//SoundHandle SoundEngine::PlaySound(const std::string& filePath, bool isAutoDelete)
//{
//    SoundPtr sound = CreateSound(filePath, isAutoDelete);
//    sound->Play();
//    return CreateHandle(sound);
//}
//
//bool SoundEngine::PlayOnce(const std::string& filePath)
//{
//    SoundPtr sound = CreateSound(filePath, true);
//    sound->Play();
//    return sound->IsPlaying();
//}
//
//SoundHandle SoundEngine::GetSound(const std::string& filePath, bool isAutoDelete)
//{
//    std::shared_ptr<Sound> sound = CreateSound(filePath, isAutoDelete);
//    return CreateHandle(sound);
//}
//
//std::shared_ptr<Sound> SoundEngine::CreateSound(const std::string& fileName, bool isAutoDelete)
//{
//    auto it = m_buffers.find(fileName);
//    if (it != m_buffers.end())
//    {
//        SoundPtr sound(CreateSound(it->second.get(), isAutoDelete));
//        AddSound(it->second, sound);
//        return sound;
//    }
//    else
//    {
//        auto buffer = CreateBuffer(fileName);
//        m_buffers[fileName] = buffer;
//        
//        SoundPtr sound(CreateSound(buffer.get(), isAutoDelete));
//        AddSound(buffer, sound);
//        return sound;
//    }
//}
//
//void SoundEngine::AddSound(std::shared_ptr<SoundBuffer> buffer, std::shared_ptr<Sound> sound)
//{
//    auto jt = m_sounds.find(buffer->GetFileName());
//    if (jt != m_sounds.end())
//    {
//        jt->second.push_back(sound);
//    }
//    else
//    {
//        auto insertResultIt = m_sounds.emplace(buffer->GetFileName(), SoundsList());
//        assert(insertResultIt.second);
//        insertResultIt.first->second.push_back(sound);
//    }
//}
//
//bool SoundEngine::DeactivateBuffer(std::shared_ptr<SoundBuffer> buffer)
//{
//    DecrementMemory(buffer->m_sizeMemory);
//    return true;
//}
//
//void SoundEngine::OnSourceCreated(SoundBuffer* buffer, Sound* sound)
//{
//    IncrementMemory(buffer->m_sizeMemory);
//    //TODO::
//}
//
//void SoundEngine::OnSourceRemoved(SoundBuffer* buffer, Sound* sound)
//{
//    assert(!m_sounds.empty());
//    
//    auto bufferIt = m_sounds.find(buffer->GetFileName());
//    assert(bufferIt != m_sounds.end());
//    
//    SoundsList& list = bufferIt->second;
//    
//    auto removedIt = std::remove_if(list.begin(), list.end(), [sound](SoundPtr ptr) { return sound == ptr.get(); });
//    assert(removedIt != list.end());
//    assert(std::distance(removedIt, list.end()) == 1);
//    list.erase(removedIt, list.end());
//}
//
//bool SoundEngine::ActivateBuffer(std::shared_ptr<SoundBuffer> buffer)
//{
//    if (buffer->IsActive())
//        return true;
//    
//    if (!buffer->LoadMemory())
//        return false;
//    
//    if (buffer->SizeMem() >= GetMaxMem()) // too big file
//    {
//        buffer->UnloadAllSources();
//        
//#ifndef NDEBUG
//        assert(false);
//#endif
//        return false;
//    }
//    
//    IncrementMemory(buffer->SizeMem());
//    
//    if (GetCurMem() >= GetMaxMem())
//    {
//        for (auto it = m_buffers.begin(); it != m_buffers.end(); ++it)
//        {
//            auto buffer = it->second;
//            auto& sources = buffer->m_sources;
//            
//            bool anyPlaying = std::any_of(sources.begin(), sources.end(), [](Sound* it)
//            {
//                return it->IsPlaying();
//            });
//            
//            if (anyPlaying || !buffer->CanBeErased())
//                continue;
//            
//            buffer->UnloadAllSources();
//        }
//    }
//    
//    
//    if (GetCurMem() >= GetMaxMem())
//    {
//#ifndef NDEBUG
//        assert(false);
//#endif
//        return false;
//    }
//    
//    return true;
//}
