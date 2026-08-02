all:
	make clean
	make build

build:
	cmake -S . -B build -Wno-dev
	cmake --build build

test: build test_db_conn test_db_pool

test_db_conn: build
	./build/test_db_conn

test_db_pool: build
	./build/test_db_pool

clean:
	rm -rf build/