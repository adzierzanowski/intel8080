CXXFLAGS = -Wall -Wpedantic -std=c++17 -O3
SOURCES = main.cc Intel8080.cc

all:
	g++ $(CXXFLAGS) $(SOURCES) -o emulator