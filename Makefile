BUILD = build
EXE = emulator
TEST = test

CXXFLAGS = -Wall -Wpedantic -O3 -std=c++17

EXE_OBJECTS_ = Intel8080.o main.o
EXE_OBJECTS = $(addprefix $(BUILD)/, $(EXE_OBJECTS_))
TEST_OBJECTS_ = Intel8080.o Intel8080Test.o test.o
TEST_OBJECTS = $(addprefix $(BUILD)/, $(TEST_OBJECTS_))

all: $(EXE) $(TEST)

clean:
	rm -rf $(BUILD)
	rm $(TEST)
	rm $(EXE)

leaks:
	leaks --atExit -- ./$(EXE)

$(EXE): $(BUILD) $(EXE_OBJECTS)
	$(CXX) $(CXXFLAGS) $(EXE_OBJECTS) -o $@

$(TEST): $(BUILD) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $@

$(BUILD)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cc: %.h

$(BUILD):
	mkdir $@
