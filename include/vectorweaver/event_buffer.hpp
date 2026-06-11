#pragma once

#include "concepts.hpp"
#include "vector_clock.hpp"
#include <unordered_map>
#include <vector>

namespace vw
{
/// @brief Estrutura genérica que representa uma mensagem retida na fila de causalidade.
/// @tparam P Tipo de dado do conteúdo da mensagem (payload).
template <Payload P>
struct BufferedMessage
{
    int sender_id{};
    P content{};
    ClockState clock_state{};
};

template <Payload P> using MessageList = std::vector<BufferedMessage<P>>;

/// @brief Gerencia a fila de mensagens adiantadas e garante a entrega causal (Causal Delivery).
/// @tparam P Tipo de dado do conteúdo da mensagem (payload).
template <Payload P>
class EventBuffer
{
public:
    EventBuffer() = default;

    /// @brief Adiciona uma mensagem na fila de espera.
    void add_to_queue(const BufferedMessage<P>& msg)
    {
        pending_queue.push_back(msg);
    }

    /// @brief Varre a fila e retorna as mensagens que respeitam a ordem causal.
    MessageList<P> pop_deliverable_messages(VectorClock& local_clock);

private:
    MessageList<P> pending_queue{};
};

template <Payload P>
__attribute__((noinline)) MessageList<P> EventBuffer<P>::pop_deliverable_messages(VectorClock& local_clock)
{
    MessageList<P> delivered{};
    auto found_any{false};

    do
    {
        found_any = false;

        for (auto it{pending_queue.begin()}; it != pending_queue.end(); ++it)
            if (local_clock.can_deliver(it->sender_id, it->clock_state))
            {
                delivered.push_back(*it);
                local_clock.update(it->clock_state);
                pending_queue.erase(it);
                
                found_any = true;
                break;
            }
    } while (found_any);

    return delivered;
}
}; // namespace vw