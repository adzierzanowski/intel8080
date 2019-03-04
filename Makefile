BUILD = build
EXE = i8080emu
TEST = test
SRC = src

CXXFLAGS = -Wall -Wpedantic -O3 -std=c++17

EXE_OBJECTS_ = Intel8080.o main.o
EXE_OBJECTS = $(addprefix $(BUILD)/, $(EXE_OBJECTS_))
TEST_OBJECTS_ = Intel8080.o Intel8080Test.o test.o
TEST_OBJECTS = $(addprefix $(BUILD)/, $(TEST_OBJECTS_))
TEST_FILES = $(addsuffix .bin, $(TEST_FILES_))

all: $(EXE)

clean:
	-rm $(EXE)
	-rm $(TEST)
	-rm tests/*.bin
	-rm tests/asm/*.bin
	-rm tests/bdos/*.bin
	-rm -rf $(BUILD)

leaks:
	leaks --atExit -- ./$(EXE)

reassemble:
	./assembleTests.sh

$(EXE): $(BUILD) $(EXE_OBJECTS)
	$(CXX) $(CXXFLAGS) $(EXE_OBJECTS) -o $@

$(TEST): $(BUILD) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $@
	./assembleTests.sh

$(BUILD)/%.o: $(SRC)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cc: %.h

$(BUILD):
	mkdir $@
