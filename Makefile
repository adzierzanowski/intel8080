BUILD = build
COV_EXCLUDE = **/usr/** **/argparser/** **/tests/**
COV_OUT = coverage.info

main: $(BUILD)/i8080
all: $(BUILD)/i8080 $(BUILD)/test
tests: $(BUILD)/test 

.PHONY: clean
clean:
	- rm -rf $(BUILD)

$(BUILD)/i8080: $(BUILD)
	cd $< && make i8080

$(BUILD)/test: $(BUILD)
	cd $< && make test

$(BUILD):
	mkdir $@
	cmake -S . -B $@

src/emu.cc.gcda: $(BUILD)/test
	./$(BUILD)/test

.PHONY: cov
cov: src/emu.cc.gcda
	lcov --capture --directory . --output-file $(COV_OUT)
	$(foreach exclude, $(COV_EXCLUDE), lcov --remove $(COV_OUT) '$(exclude)' --output-file $(COV_OUT))
	lcov --list $(COV_OUT)
