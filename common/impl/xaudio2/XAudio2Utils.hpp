#pragma once

#ifdef X2AUDIO_SOUND

#define OPSETID 1U

#include <winerror.h>
#include <windows.h>
#include <strsafe.h>

#include <iostream>
#include <string>
#include <type_traits>

template<typename... Ts> struct make_void { using type = void; };
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

template <typename T, typename S, typename = void>
struct has_output_operator : std::false_type {};

template <typename T, typename S>
struct has_output_operator<T, S, void_t<decltype(std::declval<S>() << std::declval<T>())>> : std::true_type {};

template<typename T>
class XAudio2CallResult
{
public:
    XAudio2CallResult(const T& value, bool error, const char* filename, const std::uint_fast32_t line)
	 : m_value(value)
	 , m_isError(error)
     , m_filename(filename)
	 , m_line(line)
    { }

    operator T() const { return m_value; }
    operator bool() const { return !m_isError; }
    operator std::string() const { return native_error(); }

    template<typename Elem>
    friend std::basic_ostream<Elem>& operator<<(std::basic_ostream<Elem>& stream, const XAudio2CallResult<T>& result)
    {
        if (!result.m_isError)
        {
            stream << "***SUCCESSFUL CALL*** (" << result.m_filename << ": " << result.m_line << ")\n";
            return stream;
        }

        stream << "***ERROR*** (" << result.m_filename << ": " << result.m_line << ")\n";
        stream << static_cast<std::string>(result);
        stream << std::endl;

        return stream;
    }

private:
    std::string native_error() const
    {
        if (!m_isError)
            return "";

        const DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0)
            return std::string();

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorMessageID,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&messageBuffer,
            0,
            NULL);

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);
        return message;
    }

private:
    T m_value;
    bool m_isError;
    const char* m_filename;
    const std::uint_fast32_t m_line;
};


template<typename Function, typename Stream, typename... Params>
auto x2CallImpl(const char* filename,
    const std::uint_fast32_t line,
    Stream& stream,
    Function function,
    Params ... params)
-> typename std::enable_if<std::is_same<HRESULT, decltype(function(params...))>::value, XAudio2CallResult<HRESULT>>::type
{
    static_assert(has_output_operator<XAudio2CallResult<HRESULT>, Stream>::value, "");

    HRESULT hr = function(std::forward<Params>(params)...);

    auto result = XAudio2CallResult<HRESULT>
    {
        hr,
        FAILED(hr),
        filename,
        line
    };

    if (!result)
        stream << result;

    return result;
}

template<typename Function, typename Stream, typename... Params>
auto x2CallImpl(const char* filename,
    const std::uint_fast32_t line,
    Stream& stream,
    Function function,
    Params ... params)
-> typename std::enable_if<!std::is_same<HRESULT, decltype(function(params...))>::value, XAudio2CallResult<decltype(function(params...))>>::type
{
    static_assert(has_output_operator<XAudio2CallResult<decltype(function(params...))>, Stream>::value, "");

    auto result = XAudio2CallResult<decltype(function(params...))>
    {
        function(std::forward<Params>(params)...),
        ::GetLastError() != 0,
        filename,
        line
    };

    if (!result)
        stream << result;

    return result;
}

#define x2Call(function, ...) x2CallImpl(__FILE__, __LINE__, std::cerr, function, __VA_ARGS__)

#endif
