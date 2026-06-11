#pragma once

#include "vectorweaver/strategies.hpp"
#include <unordered_map>

namespace vw
{
/// @brief Gerencia o estado de tempo lógico e valida regras de causalidade.
class VectorClock
{
public:
    explicit VectorClock(int id) :
        process_id{id}, ordering_policy{std::make_unique<StrictCausalPolicy>()}
    {
        clock[process_id] = 0;
    }

    explicit VectorClock(int id, std::unique_ptr<IOrderingPolicy> policy) :
        process_id{id}, ordering_policy{std::move(policy)} 
    {
        clock[process_id] = 0;
    }

    void tick(){ clock[process_id]++; }

    /// @brief Atualiza o relógio local com base no relógio recebido max(local, recebido)
    /// @param received_clock 
    void update(const ClockState& received_clock);


    /// @brief Verifica se a mensagem respeita a ordem causal.
    /// @param sender_id
    /// @param msg_clock
    bool can_deliver(int sender_id, const ClockState& msg_clock) const
    {
        return ordering_policy->can_deliver(sender_id, msg_clock, clock);
    }

    ClockState get_state() const { return clock; }
    
    int get_id() const { return process_id; }
    
private:
    int process_id;
    ClockState clock; // Mapeia ID do Processo -> Valor do Relógio
    std::unique_ptr<IOrderingPolicy> ordering_policy{};
};
} // namespace vw