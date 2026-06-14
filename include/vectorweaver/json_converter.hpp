#pragma once

#include "concepts.hpp"
#include "json.hpp"

namespace vw
{
/// @brief Utilitário estático para serialização e desserialização de relógios.
class JsonConverter
{
public:
    /// @brief Converte o mapa do Relógio Vetorial para um objeto JSON.
    /// @param clock_map
    static nlohmann::json map_to_json(const ClockState& clock_map)
    {
        auto j = nlohmann::json::object();
        for (const auto& [key, val] : clock_map)
            j[std::to_string(key)] = val;
        return j;
    }

    /// @brief Reconstrói o mapa numérico a partir do JSON.
    /// @param json_clock
    static ClockState json_to_map(const nlohmann::json& json_clock)
    {
        ClockState clock_map;
        for (const auto& [key, val] : json_clock.items())
            clock_map[std::stoi(key)] = val.get<int>();
        return clock_map;
    }
};
} // namespace vw