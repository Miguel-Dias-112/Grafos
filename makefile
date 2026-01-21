# Makefile para L(p,q)-Coloring
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -static

# Programa principal
TARGET = lpq_coloring
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

# Testes
TEST_TARGET = tests
TEST_SRC = tests.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)
TEST_LIBS = -lgtest -lgtest_main -pthread

# Alvos
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar testes
tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TEST_LIBS)

$(TEST_OBJ): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Executar programa principal
run: $(TARGET)
	./$(TARGET)

# Executar testes
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Executar suite específica de testes
test-distance1: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestDistance1.*

test-distance2: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestDistance2.*

test-minimization: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestMinimization.*

test-algorithms: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestGreedyAlgorithms.*

test-randomized: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestRandomized.*

test-inputs: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestInputs.*

test-performance: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestPerformance.*

test-edge: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_filter=TestEdgeCases.*

# Listar todos os testes disponíveis
list-tests: $(TEST_TARGET)
	./$(TEST_TARGET) --gtest_list_tests

# Limpar arquivos gerados
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET) resultados.csv coloracao.txt
	rm -rf test_instances

# Limpar tudo incluindo executáveis
distclean: clean
	rm -f *.exe

# Ajuda
help:
	@echo "Alvos disponíveis:"
	@echo "  make              - Compila o programa principal"
	@echo "  make tests        - Compila os testes"
	@echo "  make run          - Executa o programa principal"
	@echo "  make test         - Executa todos os testes"
	@echo "  make test-*       - Executa suite específica (distance1, distance2, etc.)"
	@echo "  make list-tests   - Lista todos os testes disponíveis"
	@echo "  make clean        - Remove arquivos temporários"
	@echo "  make distclean    - Remove tudo incluindo executáveis"
	@echo "  make help         - Mostra esta mensagem"

.PHONY: all tests run test test-distance1 test-distance2 test-minimization \
        test-algorithms test-randomized test-inputs test-performance test-edge \
        list-tests clean distclean help
