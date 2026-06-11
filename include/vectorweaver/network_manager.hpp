#pragma once

#include "strategies.hpp"
#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <zmq.hpp>

namespace vw
{
/// @brief Gerencia toda a comunicação de rede, incluindo o servidor local e o envio de mensagens para outros nós.
class NetworkManager
{
public:
    explicit NetworkManager(std::unique_ptr<IChaosStrategy> strategy) :
        context{1},
        receiver_socket{context, zmq::socket_type::pull}, 
        is_running{false},
        chaos_strategy{std::move(strategy)} 
    { /* Do nothing */ }

    ~NetworkManager() { stop(); }

    /// @brief  Inicia o servidor local (bind) e recebe uma função de callback.
    /// @param local_port 
    /// @param on_receive 
    void start_server(int local_port, std::function<void(const std::string&)> on_receive);

    /// @brief Envia uma mensagem para um nó específico, aplicando a estratégia de caos se habilitada.
    /// @param target_port
    /// @param payload
    void send_message(int target_port, const std::string& payload);

    /// @brief Para a thread de escuta e limpa os recursos
    void stop()
    {
        if (is_running)
        {
            is_running = false;
            if (listener_thread.joinable())
                listener_thread.join();
            receiver_socket.close();
        }
    }
private:
    zmq::context_t context;
    zmq::socket_t receiver_socket;
    
    std::atomic<bool> is_running;
    std::thread listener_thread;
    std::unique_ptr<IChaosStrategy> chaos_strategy{};
};
} // namespace vw