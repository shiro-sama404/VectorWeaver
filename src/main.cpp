#include "vectorweaver/cli_handler.hpp"
#include "vectorweaver/event_buffer.hpp"
#include "vectorweaver/message_handler.hpp"
#include "vectorweaver/network_manager.hpp"
#include "vectorweaver/strategies.hpp"
#include "vectorweaver/vector_clock.hpp"

#include <iostream>
#include <memory>
#include <mutex>
#include <string>

int main(int argc, char* argv[])
{
    auto node_id{-1};
    auto local_port{-1};

    try
    {
        for (auto i{1}; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--id" && i + 1 < argc) node_id = std::stoi(argv[++i]);
            if (arg == "--port" && i + 1 < argc) local_port = std::stoi(argv[++i]);
        }
    }
    catch (const std::exception&)
    {
        std::cerr << "[ERRO] Argumentos invalidos. Use apenas numeros para ID e Porta.\n";
        return 1;
    }

    if (node_id == -1 || local_port == -1)
    {
        std::cerr << "Uso correto: vector_weaver --id <ID_No> --port <Porta>\n";
        return 1;
    }

    std::mutex state_mutex{};

    vw::VectorClock local_clock{node_id};
    vw::EventBuffer<std::string> event_buffer{};
    vw::NetworkManager network{std::make_unique<vw::UniformChaos>(0, 5000)};

    vw::MessageHandler msg_handler{local_clock, event_buffer, state_mutex};

    network.start_server(local_port, [&msg_handler](const std::string& payload) 
    {
        msg_handler.handle_incoming_message(payload);
    });

    std::cout << "===========================================\n";
    std::cout << " VectorWeaver - No ID: " << node_id << " | Porta: " << local_port << "\n";
    
    vw::CliHandler cli{local_clock, network, state_mutex};
    cli.run();

    return 0;
}