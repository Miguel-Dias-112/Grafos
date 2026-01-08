# Makefile
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

TARGET = lpq_coloring
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) resultados.csv coloracao.txt

run: $(TARGET)
	./$(TARGET)
