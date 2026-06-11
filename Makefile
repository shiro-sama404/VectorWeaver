CXX = C:/msys64/mingw64/bin/g++.exe
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Iinclude/vectorweaver -I./deps -I"C:/msys64/mingw64/include"
LDFLAGS = -L"C:/msys64/mingw64/lib" -lzmq

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(BIN_DIR)/vector_weaver.exe

# Diretório de bibliotecas do MSYS2 para a linkagem
LDFLAGS = -L"C:/msys64/mingw64/lib" -lzmq

.PHONY: all clean directories run

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Compilação concluída: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Limpeza concluída."

run: all
	@./$(TARGET)