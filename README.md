# 🕸️ VectorWeaver

**Simulador Interativo de Relógios Vetoriais e Ordenação de Eventos**

O **VectorWeaver** é uma biblioteca e simulador em **C++20** de um sistema distribuído, projetado para demonstrar na prática os conceitos de ordenação parcial de eventos, quebra de causalidade e resolução de estado usando Relógios Vetoriais (Vector Clocks).

Projeto desenvolvido para a disciplina de **Computação Distribuída** na **Universidade Federal de Mato Grosso do Sul (UFMS)**.

---

## 🎯 Objetivo do Projeto

Em sistemas distribuídos reais, o tempo físico sofre com *clock drift* e a rede impõe latências imprevisíveis. O VectorWeaver simula um ambiente assíncrono de troca de mensagens onde pacotes sofrem atrasos baseados em heurísticas matemáticas (caos de rede). Esta API avalia anomalias causais e retém mensagens adiantadas no futuro em um *buffer* temporário genérico, garantindo a entrega estrita (Causal Delivery).

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C++20 (Necessário devido a utilização intensiva de *Concepts*).
* **Interface Gráfica:** Dear ImGui (Renderização de estado e topologia em tempo real via OpenGL/GLFW). *[Em Implementação]*
* **Comunicação de Rede:** [ZeroMQ (cppzmq)](https://zeromq.org/) para abstração assíncrona de sockets TCP e troca de mensagens P2P.
* **Serialização:** [`nlohmann/json`](https://github.com/nlohmann/json) para estruturação leve dos pacotes e vetores em trânsito.
* **Compilação e Build:** CMake.

## 🚀 Como Compilar e Executar (Ambiente MSYS2/MinGW)

### Pré-requisitos
Certifique-se de possuir o compilador C++20 e as dependências instaladas via gerenciador de pacotes `pacman`:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-zeromq mingw-w64-x86_64-glfw
```

### Passo 1: Compilação via CMake

Na raiz do repositório, crie o diretório de build e compile:

```
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

Isso gerará o executável VectorWeaver.exe dentro da pasta /bin.

### Passo 2: Simulando a Malha (Processos Distribuídos)

Inicialize três instâncias do sistema para compor a topologia da rede:

Terminal 1 (Nó P0):
```.\bin\VectorWeaver.exe --id 0 --port 5001```

Terminal 2 (Nó P1):
```.\bin\VectorWeaver.exe --id 1 --port 5002```

Terminal 3 (Nó P2):
```.\bin\VectorWeaver.exe --id 2 --port 5003```

## 📂 Arquitetura da API (**namespace vw**)

O núcleo do VectorWeaver foi desacoplado em uma biblioteca (include/vectorweaver/), garantindo flexibilidade para integração em motores gráficos ou sistemas CLI:

* **vector_clock.hpp**: Máquina de estados desacoplada que delega regras de validação para as Políticas de Ordenação.

* **strategies.hpp**: Implementação do padrão Strategy para customização em tempo de execução da latência de rede (**UniformChaos**, **NoChaos**) e tratamento causal (**StrictCausalPolicy**, **RelaxedDeliveryPolicy**).

* **event_buffer.hpp**: Fila genérica de retenção protegida por C++ Concepts (Payload P) restritivos para garantir segurança de memória no tráfego de dados estruturados.

* **network_manager.hpp**: Camada de abstração do ZeroMQ assíncrono.


## 👥 Autores

| [<img loading="lazy" src="https://avatars.githubusercontent.com/u/68046889?v=4" width=115><br><sub>Arthur de Andrade</sub>](https://github.com/shiro-sama404) |  [<img loading="lazy" src="https://avatars.githubusercontent.com/u/144397400?v=4" width=115><br><sub>Jenniffer Checchia</sub>](https://github.com/Jenn-Checchia) |
| :---: | :---: |

## 📄 Referências
* FIDGE, C. J. **Timestamps in message-passing systems that preserve the partial ordering.** 1988.
* MATTERN, F. **Virtual time and global states of distributed systems.** 1989.