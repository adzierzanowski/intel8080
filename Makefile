BUILD = build
EXE = emulator
DUMP = dump
CPUDIAG = cpudiag.bin

CXXFLAGS = -Wall -Wpedantic -std=c++17 -g
OBJECTS = $(addprefix $(BUILD)/, main.o Intel8080.o)

all: $(EXE)
	du -h $<

clean:
	rm -rf $(BUILD)
	rm $(EXE)
	rm $(DUMP)
	rm tests

leaks:
	leaks --atExit -- ./$(EXE)

tests: $(BUILD)/tests.o $(BUILD)/Intel8080.o
	$(CXX) $(CXXFLAGS) $(BUILD)/Intel8080.o $(BUILD)/tests.o -o tests

$(DUMP): $(EXE)
	./$< $(CPUDIAG) > $(DUMP)


$(EXE): $(BUILD) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

$(BUILD)/main.o: main.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD)/%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD):
	mkdir $@
