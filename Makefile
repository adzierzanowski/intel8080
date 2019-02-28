CXXFLAGS = -Wall -Wpedantic -std=c++17 -O3
OBJECTS = main.o Intel8080.o
EXE = emulator

%.o: %.cc
	g++ -c $(CXXFLAGS) $< -o $@

$(EXE): $(OBJECTS)
	g++ $(CXXFLAGS) $(OBJECTS) -o emulator

all: $(EXE)
