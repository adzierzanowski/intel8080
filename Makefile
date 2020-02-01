BUILD = build
COV_EXCLUDE = **/usr/** **/argparser/** **/tests/**
COV_OUT = coverage.info

.PHONY: main
main: $(BUILD)
	cd $< && make i8080

.PHONY: tests
tests: $(BUILD)
	cd $< && make test

.PHONY: all
all: main tests

.PHONY: clean
clean:
	- rm -rf $(BUILD)

.PHONY: test
test: rmgcda
	./$(BUILD)/test

.PHONY: rmgcda
rmgcda:
	find . | grep ".gcda" | xargs rm

$(BUILD):
	mkdir $@
	cmake -S . -B $@

src/emu.cc.gcda: tests
	./$(BUILD)/test

.PHONY: cov
cov: src/emu.cc.gcda
	lcov --capture --directory . --output-file $(COV_OUT)
	$(foreach exclude, $(COV_EXCLUDE), lcov --remove $(COV_OUT) '$(exclude)' --output-file $(COV_OUT))
	lcov --list $(COV_OUT)
