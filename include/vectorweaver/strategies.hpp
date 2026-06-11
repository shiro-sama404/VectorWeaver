#pragma once

#include "concepts.hpp"
#include <chrono>
#include <random>
#include <thread>
#include <unordered_map>

namespace vw
{
// ========================================================================
// ESTRATÉGIAS DE CAOS (REDE)
// ========================================================================

/// @brief Contrato para simulação de anomalias e latência de rede.
class IChaosStrategy
{
public:
    virtual ~IChaosStrategy() = default;

    /// @brief Aplica a retenção/atraso na thread atual.
    virtual void apply_delay() const = 0;
};

/// @brief Rede perfeita, sem atrasos. Ideal para testes locais.
class NoChaos : public IChaosStrategy
{
public:
    void apply_delay() const override {}
};

/// @brief Simula flutuações randômicas de rede (0 a 5 segundos).
class UniformChaos : public IChaosStrategy
{
public:
    UniformChaos(int min_ms = 0, int max_ms = 5000) : 
        min_delay{min_ms}, 
        max_delay{max_ms}, 
        gen{rd()} 
    { /* Do nothing */ }

    void apply_delay() const override
    {
        std::uniform_int_distribution<> dist{min_delay, max_delay};
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));
    }
    
private:
    int min_delay;
    int max_delay;
    mutable std::random_device rd;
    mutable std::mt19937 gen;
};

// ========================================================================
// POLÍTICAS DE ORDENAÇÃO (CAUSALIDADE)
// ========================================================================

/// @brief Contrato para validação da entrega de mensagens.
class IOrderingPolicy
{
public:
    virtual ~IOrderingPolicy() = default;

    /// @brief Avalia se uma mensagem pode ser entregue com base no estado atual.
    /// @param sender_id ID do remetente da mensagem
    /// @param msg_clock Estado do relógio vetorial da mensagem
    /// @param local_clock Estado do relógio vetorial local
    /// @return true se a mensagem pode ser entregue, false caso contrário
    virtual bool can_deliver(int sender_id, 
                             const ClockState& msg_clock, 
                             const ClockState& local_clock) const = 0;
};

/// @brief Garante FIFO e falha em eventos não causais.
class StrictCausalPolicy : public IOrderingPolicy
{
public:
    bool can_deliver(int sender_id, 
                        const ClockState& msg_clock, 
                        const ClockState& local_clock) const override
    {
        auto local_sender_val{local_clock.count(sender_id) ? local_clock.at(sender_id) : 0};
        auto msg_sender_val{msg_clock.count(sender_id) ? msg_clock.at(sender_id) : 0};

        // Rejeita se não for a próxima
        if (msg_sender_val != local_sender_val + 1)
            return false;

        // Rejeita se houver dependência de terceiros no futuro
        for (const auto& [id, msg_val] : msg_clock)
            if (id != sender_id)
            {
                auto local_val{local_clock.count(id) ? local_clock.at(id) : 0};
                if (msg_val > local_val)
                    return false;
            }

        return true;
    }
};

/// @brief Tolera eventos silenciosos e foca apenas em concorrência.
class RelaxedDeliveryPolicy : public IOrderingPolicy
{
public:
    bool can_deliver(int sender_id, 
                        const ClockState& msg_clock, 
                        const ClockState& local_clock) const override
    {
        auto local_sender_val{local_clock.count(sender_id) ? local_clock.at(sender_id) : 0};
        auto msg_sender_val{msg_clock.count(sender_id) ? msg_clock.at(sender_id) : 0};

        // Rejeita apenas pacotes duplicados/antigos, permite pulos numéricos
        if (msg_sender_val <= local_sender_val)
            return false;

        for (const auto& [id, msg_val] : msg_clock)
            if (id != sender_id)
            {
                auto local_val{local_clock.count(id) ? local_clock.at(id) : 0};
                if (msg_val > local_val)
                    return false;
            }

        return true;
    }
};
}