all: build
	cd build && make

tests:
	cd build && make test

build:
	- mkdir $@
	cmake -B build

clean:
	- rm -rf build
