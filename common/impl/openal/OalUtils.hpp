#pragma once

#ifdef OPENAL_SOUND

#include <OpenAL/OpenAL.h>

#include <string>
#include <type_traits>

namespace details
{
    bool check_al_errors(const std::string& filename, const std::uint_fast32_t line);
    bool check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);
}

template<typename T>
struct OpenALCallResult
{
    T value;
    bool isError;
    
    OpenALCallResult(const T& v, bool error):value(v), isError(error)
    { }
    
    operator T() const { return value; }
    operator bool() const { return !isError; }
};

template<typename T>
constexpr ALCdevice* get_device(T arg)
{
    if constexpr(std::is_same<T, ALCdevice*>::value)
    {
        return arg;
    }
    else
    {
        return nullptr;
    }
}

template<typename T, typename ... Args>
constexpr ALCdevice* get_device(T arg, Args&& ... args)
{
    if constexpr (std::is_same<T, ALCdevice*>::value)
        return arg;
    
    return get_device(args...);
}


template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params ... params)
->typename std::enable_if<!std::is_same<void,decltype(function(params...))>::value, OpenALCallResult<decltype(function(params...))>>::type
{
    ALCdevice* device = get_device(params...);
    
    using namespace details;
    
    return OpenALCallResult<decltype(function(params...))>
    {
        function(std::forward<Params>(params)...),
        device ? check_alc_errors(filename,line,device) : check_al_errors(filename,line)
    };
}
template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params ... params)
->typename std::enable_if<std::is_same<void,decltype(function(params...))>::value, bool>::type
{
    ALCdevice* device = get_device(params...);
    
    using namespace details;
    
    function(std::forward<Params>(params)...);
    return device ? check_alc_errors(filename,line,device) : check_al_errors(filename,line);
}

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)

#endif
