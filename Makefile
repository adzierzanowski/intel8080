BUILD = build

main: $(BUILD)
	cd $< && make i8080

all: $(BUILD) 
	cd $< && make

tests: $(BUILD)
	cd $< && make test

$(BUILD):
	mkdir $@
	cmake -S . -B $@

clean:
	- rm -rf $(BUILD)
