#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

#include "InternalBuffer.hpp"
#include "InternalEngine.hpp"

class SoundHandle;

template<typename TSound, typename TBuffer>
class SoundEngine : public InternalEngine
{
private:
    using SoundPtr = std::shared_ptr<TSound>;
    using BufferPtr = std::shared_ptr<TBuffer>;
    using SoundsList = std::vector<SoundPtr>;

public:
    SoundEngine()
        : m_maxMem(15.f)
        , m_curMem(0.f)
    {
        static_assert(std::is_base_of<InternalSound, TSound>::value, "");
        static_assert(std::is_base_of<InternalBuffer, TBuffer>::value, "");
    }
    
    ~SoundEngine()
    {
        Reset();
    }
    
private:
    SoundHandle PlaySound(const std::string& filePath, bool isAutoDelete) override final
    {
        SoundPtr sound = CreateSound(filePath, isAutoDelete);
        sound->Play();
        return SoundHandle(sound);
    }
    
    SoundHandle GetSound(const std::string& filePath, bool isAutoDelete) override final
    {
        std::shared_ptr<TSound> sound = CreateSound(filePath, isAutoDelete);
        return SoundHandle(sound);
    }
    
    bool PlayOnce(const std::string& filePath) override final
    {
        SoundPtr sound = CreateSound(filePath, true);
        sound->Play();
        return sound->IsPlaying();
    }
protected:
    void Update(float deltaTime) override
    {
        auto it = m_sounds.begin();
        while (it != m_sounds.end())
        {
            SoundsList& list = it->second;

            for (std::size_t i = 0; i < list.size();)
            {
                if (list[i]->m_isAutoDelete && !list[i]->IsPlaying() && list[i]->IsValid()) // played once
                    list[i]->Delete();
                else
                    ++i;
            }

            ++it;
        }
    }
    
private:
    float GetMaxMem() const { return m_maxMem; };
    float GetCurMem() const { return m_curMem; };
    
    bool ActivateBuffer(BufferPtr buffer)
    {
        if (buffer->IsActive())
            return true;

        if (!buffer->LoadMemory())
            return false;

        if (buffer->SizeMem() >= GetMaxMem()) // too big file
        {
            buffer->UnloadAllSources();

    #ifndef NDEBUG
            assert(false);
    #endif
            return false;
        }

        IncrementMemory(buffer->SizeMem());

        if (GetCurMem() >= GetMaxMem())
        {
            for (auto it = m_buffers.begin(); it != m_buffers.end(); ++it)
            {
                auto buffer = it->second;
                auto& sources = buffer->m_sources;

                bool anyPlaying = std::any_of(sources.begin(), sources.end(), [](TSound* it)
                    {
                        return it->IsPlaying();
                    });

                if (anyPlaying || !buffer->CanBeErased())
                    continue;

                buffer->UnloadAllSources();
            }
        }


        if (GetCurMem() >= GetMaxMem())
        {
    #ifndef NDEBUG
            assert(false);
    #endif
            return false;
        }

        return true;
    }
    bool DeactivateBuffer(BufferPtr buffer)
    {
        DecrementMemory(buffer->m_sizeMemory);
        return true;
    }
    
    void IncrementMemory(float sizeMem)
    {
        m_curMem += sizeMem;
    }
    void DecrementMemory(float sizeMem)
    {
        m_curMem -= sizeMem;
        assert(m_curMem >= 0);
    }
    
public:
    void OnSourceCreated(InternalBuffer* buffer, InternalSound* sound) override
    {
        IncrementMemory(buffer->GetSizeMem());
        //TODO::
    }
    
    void OnSourceRemoved(InternalBuffer* buffer, InternalSound* sound) override
    {
        assert(!m_sounds.empty());

        auto bufferIt = m_sounds.find(buffer->GetFileName());
        assert(bufferIt != m_sounds.end());

        SoundsList& list = bufferIt->second;

        auto removedIt = std::remove_if(list.begin(), list.end(), [sound](SoundPtr ptr) { return sound == ptr.get(); });
        assert(removedIt != list.end());
        assert(std::distance(removedIt, list.end()) == 1);
        list.erase(removedIt, list.end());
    }

protected:
    virtual std::shared_ptr<TSound> CreateSound(TBuffer* buffer, bool isAutoDelete) = 0;
    virtual std::shared_ptr<TBuffer> CreateBuffer(const std::string& file) = 0;
    
    virtual bool IsValid() const = 0;
    
    void Reset()
    {
        m_buffers.clear();
        for (auto& filePathToSounds : m_sounds)
            assert(filePathToSounds.second.empty());
        m_sounds.clear();
    }
    
private:
    void AddSound(BufferPtr buffer, SoundPtr sound)
    {
        auto jt = m_sounds.find(buffer->GetFileName());
        if (jt != m_sounds.end())
        {
            jt->second.push_back(sound);
        }
        else
        {
            auto insertResultIt = m_sounds.emplace(buffer->GetFileName(), SoundsList());
            assert(insertResultIt.second);
            insertResultIt.first->second.push_back(sound);
        }
    }
    
    SoundPtr CreateSound(const std::string& fileName, bool isAutoDelete)
    {
        auto it = m_buffers.find(fileName);
        if (it == m_buffers.end())
            m_buffers[fileName] = CreateBuffer(fileName);

        BufferPtr buffer_ptr = m_buffers[fileName];
        SoundPtr sound_ptr(CreateSound(buffer_ptr.get(), isAutoDelete));

        buffer_ptr->AttachSource(sound_ptr.get());

        AddSound(buffer_ptr, sound_ptr);
        return sound_ptr;
    }

    // loaded buffer => all created sources from it
    std::unordered_map<std::string, SoundsList> m_sounds;
    // file path => loaded buffer
    std::unordered_map<std::string, BufferPtr> m_buffers;
    
    float m_maxMem;
    float m_curMem;
};
