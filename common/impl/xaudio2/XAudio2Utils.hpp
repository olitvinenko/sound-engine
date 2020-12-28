#pragma once

#ifdef X2AUDIO_SOUND

template<typename T, typename ... Args>
auto Call(T&& func, Args && ... args) -> typename std::enable_if<!std::is_member_function_pointer<T>::value, decltype(func(args...))>::type
{
    return std::forward<T>(func)(std::forward<Args>(args)...);
}

template <class T, class Type, class T1, class... Args>
auto Call(Type T::* f, T1* t1, Args&&... args) -> typename std::enable_if<!std::is_member_function_pointer<T>::value, decltype((t1->*f)(std::forward<Args>(args)...))>::type
{
    return (t1->*f)(std::forward<Args>(args)...);
}

#endif
