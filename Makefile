SRC = src
BUILD = build

CXXFLAGS = -std=c++17 -Wall -Wpedantic -DDEBUG

EXE = $(addprefix $(BUILD)/, emu test)
OBJECTS = $(addprefix $(BUILD)/, opcode.o ram.o cpu.o emu.o)
TEST_OBJECTS = test_cpu.o \
test_opcodes_0x0x.o \
test_opcodes_0x1x.o \
test_opcodes_0x2x.o \
test_opcodes_0x3x.o \
test_opcodes_mov.o \
test_opcodes_add.o \
test_opcodes_sub.o \
test_opcodes_bitwise.o \
test_opcodes_0xcx.o

all: $(EXE)
clean:
	- rm -rf $(BUILD)

$(BUILD)/emu: $(addprefix $(BUILD)/, main.o) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $@

$(BUILD)/test: $(addprefix $(BUILD)/, test.o) $(OBJECTS) $(addprefix $(BUILD)/, $(TEST_OBJECTS))
	$(CXX) $(CXXFLAGS) -lcriterion $(OBJECTS) $(addprefix $(BUILD)/, $(TEST_OBJECTS)) $< -o $@

$(BUILD)/test_%.o: $(SRC)/tests/test_%.cc
	$(CXX) $(CXXFLAGS) -Wno-keyword-macro -c $< -o $@

$(BUILD)/%.o: $(SRC)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	- mkdir build
