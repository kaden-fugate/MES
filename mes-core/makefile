all:
	make clean
	make build
	make run

build:
	cmake -S . -B build
	cmake --build build

run:
	./build/mes_server

clean:
	rm -rf build/