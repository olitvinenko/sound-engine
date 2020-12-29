#pragma once

#include <iostream>
#include <type_traits>

template<typename... Ts> struct make_void { using type = void; };
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

template <typename T, typename S, typename = void>
struct has_output_operator : std::false_type {};

template <typename T, typename S>
struct has_output_operator<T, S, void_t<decltype(std::declval<S>() << std::declval<T>())>> : std::true_type {};

template<typename R, typename ... Args>
R Call(R(*function)(Args...), Args&&... args)
{
    return function(std::forward<Args>(args)...);
}

template<typename R, typename O, typename ... Args>
R Call(R(O::* function)(Args...), O* obj, Args&&... args)
{
    return (obj->*function)(std::forward<Args>(args)...);
}

