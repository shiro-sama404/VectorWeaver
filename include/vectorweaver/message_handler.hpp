#pragma once

#include "event_buffer.hpp"
#include "vector_clock.hpp"
#include <mutex>
#include <string>

namespace vw
{
/// @brief Controla a recepção de mensagens e aplica as regras de rede no estado local.
class MessageHandler
{
public:
   MessageHandler(VectorClock& clock, EventBuffer<std::string>& buffer, std::mutex& mutex):
        local_clock(clock),
        event_buffer(buffer),
        state_mutex(mutex)
    { /* Do nothing */ }

    /// @brief Processa a string bruta vinda da rede.
    /// @param payload Conteúdo da mensagem a ser processada
    void handle_incoming_message(const std::string& payload);

private:
    VectorClock& local_clock;
    EventBuffer<std::string>& event_buffer;
    std::mutex& state_mutex;
};
} // namespace vw