#include "vectorweaver/network_manager.hpp"
#include <chrono>
#include <iostream>
#include <random>

namespace vw
{
void NetworkManager::start_server(int local_port, std::function<void(const std::string&)> on_receive)
{
    receiver_socket.bind("tcp://*:" + std::to_string(local_port));
    is_running = true;

    listener_thread = std::thread([this, on_receive]() {
        while (is_running)
        {
            zmq::message_t message;
            auto res = receiver_socket.recv(message, zmq::recv_flags::dontwait);
            
            if (res)
            {
                std::string payload(static_cast<char*>(message.data()), message.size());
                on_receive(payload);
            }
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
}

void NetworkManager::send_message(int target_port, const std::string& payload)
    {
        std::thread([this, target_port, payload]() 
        {
            chaos_strategy->apply_delay();

            try
            {
                zmq::socket_t sender_socket{context, zmq::socket_type::push};
                sender_socket.connect("tcp://127.0.0.1:" + std::to_string(target_port));
                
                zmq::message_t msg{payload.data(), payload.size()};
                sender_socket.send(msg, zmq::send_flags::none);
            }
            catch (const zmq::error_t& e)
            {
                std::cerr << "[ERRO de Rede] Falha ao enviar pacote: " << e.what() << "\n";
            }
            
        }).detach();
    }
} // namespace vw