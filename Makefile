# ==============================================================================
# Configurações do Compilador e Flags
# ==============================================================================
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Dependências e Includes
# Certifique-se de que o caminho do ZeroMQ e nlohmann/json estão mapeados aqui
INCLUDES = -I$(INC_DIR) -I./deps
LDFLAGS = -lzmq

# Arquivos fonte e objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Nome do executável final
TARGET = $(BIN_DIR)/vector_weaver.exe

# ==============================================================================
# Regras de Compilação
# ==============================================================================

# Regra principal
all: directories $(TARGET)

# Cria os diretórios bin/ e obj/ se não existirem
directories:
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

# Regra de linkagem (Gera o .exe)
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo Compilacao concluida com sucesso: $(TARGET)

# Regra de compilação (Gera os .o a partir dos .cpp)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Limpa os arquivos gerados
clean:
	@if exist "$(OBJ_DIR)" del /Q /S "$(OBJ_DIR)\*.o"
	@if exist "$(BIN_DIR)\$(TARGET)" del /Q "$(BIN_DIR)\vector_weaver.exe"
	@echo Arquivos limpos.

# Evita conflitos com arquivos chamados 'all', 'clean' ou 'directories'
.PHONY: all clean directories