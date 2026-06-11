#include "json.hpp"
#include "vectorweaver/cli_handler.hpp"
#include "vectorweaver/json_converter.hpp"
#include <iostream>
#include <string>

namespace vw
{
void CliHandler::print_menu() const
{
    std::cout << "===========================================\n";
    std::cout << " Comandos disponiveis:\n";
    std::cout << " 1: Evento Interno (Incrementar relogio)\n";
    std::cout << " 2: Enviar Mensagem para outro No\n";
    std::cout << " 3: Ver Estado (Relogio Atual)\n";
    std::cout << "===========================================\n";
}

void CliHandler::handle_internal_event()
{
    std::lock_guard<std::mutex> lock{state_mutex};
    local_clock.tick();
    std::cout << "[INFO] Evento interno registrado.\n";
}

void CliHandler::handle_send_message()
{
    std::string port_str{};
    auto target_port{0};
    std::string msg_text{};

    std::cout << "Porta de destino (Ex: 5002): ";
    std::getline(std::cin, port_str);
    
    try 
    {
        target_port = std::stoi(port_str);
    } 
    catch (const std::exception&) 
    {
        std::cout << "[ERRO] Porta invalida. Operacao cancelada.\n";
        return;
    }

    std::cout << "Mensagem a ser enviada: ";
    std::getline(std::cin, msg_text);

    std::string payload{};
    {
        std::lock_guard<std::mutex> lock{state_mutex};
        local_clock.tick(); 
        
        auto j = nlohmann::json::object();
        j["sender_id"] = local_clock.get_id();
        j["content"] = msg_text;
        j["clock_state"] = JsonConverter::map_to_json(local_clock.get_state());
        payload = j.dump();
    }

    network.send_message(target_port, payload);
    std::cout << "[REDE] Mensagem lancada (Aplicando diretrizes da politica de rede).\n";
}

void CliHandler::handle_print_state()
{
    std::lock_guard<std::mutex> lock{state_mutex};
    std::cout << "[ESTADO] Relogio Vetorial Local: { ";
    
    for (const auto& [id, val] : local_clock.get_state())
        std::cout << "P" << id << ":" << val << " ";
        
    std::cout << "}\n";
}

void CliHandler::run()
{
    print_menu();

    std::string choice{};
    while (true)
    {
        std::cout << "\nVectorWeaver> ";
        std::getline(std::cin, choice);

        if (choice == "1")
            handle_internal_event();
        else if (choice == "2")
            handle_send_message();
        else if (choice == "3")
            handle_print_state();
        else if (!choice.empty())
            std::cout << "Comando invalido.\n";
    }
}
} // namespace vw