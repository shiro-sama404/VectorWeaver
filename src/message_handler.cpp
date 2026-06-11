#include "vectorweaver/json_converter.hpp"
#include "vectorweaver/message_handler.hpp"
#include <iostream>

namespace vw
{
using json = nlohmann::json;

void MessageHandler::handle_incoming_message(const std::string& payload)
{
    try
    {
        auto j = nlohmann::json::parse(payload);
        BufferedMessage<std::string> msg{};
        msg.sender_id = j["sender_id"];
        msg.content = j["content"];
        msg.clock_state = JsonConverter::json_to_map(j["clock_state"]);

        std::lock_guard<std::mutex> lock{state_mutex};

        std::cout << "\n[REDE] Pacote recebido de P" << msg.sender_id << " -> Verificando causalidade...\n";

        if (local_clock.can_deliver(msg.sender_id, msg.clock_state))
        {
            local_clock.update(msg.clock_state);
            std::cout << ">>> [MENSAGEM ENTREGUE] P" << msg.sender_id << " diz: " << msg.content << "\n";

            auto unlocked_msgs{event_buffer.pop_deliverable_messages(local_clock)};
            
            for (const auto& u_msg : unlocked_msgs)
                std::cout << ">>> [BUFFER LIBERADO] P" << u_msg.sender_id << " diz: " << u_msg.content << "\n";
        }
        else
        {
            std::cout << "[! CAUSALIDADE VIOLADA !] A mensagem de P" << msg.sender_id << " foi retida no buffer.\n";
            event_buffer.add_to_queue(msg);
        }

        std::cout << "\nVectorWeaver> ";
        std::cout.flush();
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n[ERRO] Falha no tratamento do pacote: " << e.what() << "\n";
    }
}
} // namespace vw