BUILD = build
EXE = emulator
DUMP = dump
CPUDIAG = cpudiag.bin

CXXFLAGS = -Wall -Wpedantic -std=c++17 -O3 -g
OBJECTS = $(addprefix $(BUILD)/, main.o Intel8080.o)

all: $(EXE)
	du -h $<

clean:
	rm -rf $(BUILD)
	rm $(EXE)
	rm $(DUMP)

leaks:
	leaks --atExit -- ./$(EXE)

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
