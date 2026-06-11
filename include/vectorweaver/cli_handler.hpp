#pragma once

#include <mutex>
#include "vector_clock.hpp"
#include "network_manager.hpp"

namespace vw
{
/// @brief Gerencia a interface de linha de comando e a interação do usuário.
class CliHandler
{
public:
    CliHandler(VectorClock& clock, NetworkManager& net, std::mutex& mutex) :
        local_clock{clock}, network{net}, state_mutex{mutex}
    { /* Do nothing */}

    /// @brief Inicia o loop infinito do menu interativo.
    void run();

private:
    VectorClock& local_clock;
    NetworkManager& network;
    std::mutex& state_mutex;

    void print_menu() const;
    void handle_internal_event();
    void handle_send_message();
    void handle_print_state();
};
}