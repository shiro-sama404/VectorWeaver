#pragma once

#include <type_traits>
#include <concepts>

namespace vw
{
using ClockState = std::unordered_map<int, int>;

/// @brief Garante que o tipo T pode transitar pela rede e ser armazenado no buffer.
/// Para um payload ser válido, ele deve ser construível por padrão e copiável.
template <typename T>
concept Payload = std::is_default_constructible_v<T> && 
                    std::is_copy_constructible_v<T> && 
                    std::is_copy_assignable_v<T>;
}