BUILD = build
EXE = emulator

CXXFLAGS = -Wall -Wpedantic -std=c++17 -O3 -g
OBJECTS = $(addprefix $(BUILD)/, main.o Intel8080.o)

all: $(EXE)
	du -h $<

clean:
	rm -rf $(BUILD)
	rm $(EXE)

leaks:
	leaks --atExit -- ./$(EXE)

$(EXE): $(BUILD) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

$(BUILD)/main.o: main.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD)/%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD):
	mkdir $@
