# 🕸️ VectorWeaver

**Simulador Interativo de Relógios Vetoriais e Ordenação de Eventos**

O **VectorWeaver** é uma implementação em C++ de um sistema distribuído simulado, projetado para demonstrar na prática os conceitos de ordenação parcial de eventos, quebra de causalidade e resolução de estado usando Relógios Vetoriais (Vector Clocks).

Projeto desenvolvido como requisito para a disciplina de **Computação Distribuída** na **Universidade Federal de Mato Grosso do Sul (UFMS)**.

---

## 🎯 Objetivo do Projeto

Em sistemas distribuídos reais, o tempo físico sofre com *clock drift* e a rede impõe latências imprevisíveis. O VectorWeaver simula um ambiente de troca de mensagens onde pacotes sofrem atrasos aleatórios (caos de rede). Utilizando a regra matricial de Relógios Vetoriais, o sistema é capaz de identificar anomalias causais e reter mensagens no futuro em um *buffer* temporário, entregando-as apenas quando a ordem correta for restabelecida.

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C++17
* **Comunicação de Rede:** [ZeroMQ (libzmq)](https://zeromq.org/) para abstração de sockets TCP e troca de mensagens P2P.
* **Serialização:** [`nlohmann/json`](https://github.com/nlohmann/json) para estruturação leve dos pacotes e vetores em trânsito.
* **Compilação e Build:** Makefile padrão focado em ambiente Windows (MinGW-w64).

## 🚀 Como Compilar e Executar (Ambiente Windows)

### Pré-requisitos
1. Compilador `g++` via MSYS2/MinGW configurado nas variáveis de ambiente.
2. Ferramenta `make` instalada.
3. Biblioteca dinâmica do ZeroMQ disponível no sistema.

### Passo 1: Compilação
Na raiz do repositório, abra o terminal e execute:
`make`

Isso gerará o executável `vector_weaver.exe` dentro da pasta `/bin`.

### Passo 2: Simulando os Nós (Processos Distribuídos)
Para demonstrar o funcionamento do Relógio Vetorial, inicialize três instâncias do sistema em **três terminais separados**. O sistema exige o ID do nó e a porta TCP local:

**Terminal 1 (Nó A):**
`.\bin\vector_weaver.exe --id 0 --port 5001`

**Terminal 2 (Nó B):**
`.\bin\vector_weaver.exe --id 1 --port 5002`

**Terminal 3 (Nó C):**
`.\bin\vector_weaver.exe --id 2 --port 5003`

A partir deste momento, os terminais estarão prontos para enviar mensagens uns aos outros, anexando o Relógio Vetorial atualizado a cada requisição.

## 📂 Arquitetura do Sistema

A base de código foi estruturada separando a lógica de estados da infraestrutura de rede:

* `src/vector_clock.cpp`: Máquina de estados isolada contendo a lógica matemática de atualização do vetor.
* `src/network_manager.cpp`: Gerenciamento do ZeroMQ e injeção de latência simulada ("rede caótica").
* `src/event_buffer.cpp`: Fila de retenção baseada em causalidade (Delivery Causal).

## 👥 Autores

| [<img loading="lazy" src="https://avatars.githubusercontent.com/u/68046889?v=4" width=115><br><sub>Arthur de Andrade</sub>](https://github.com/shiro-sama404) |  [<img loading="lazy" src="https://avatars.githubusercontent.com/u/144397400?v=4" width=115><br><sub>Jenniffer Checchia</sub>](https://github.com/Jenn-Checchia) |
| :---: | :---: |

## 📄 Referências
* FIDGE, C. J. *Timestamps in message-passing systems that preserve the partial ordering.* 1988.
* MATTERN, F. *Virtual time and global states of distributed systems.* 1989.