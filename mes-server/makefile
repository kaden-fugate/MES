all:
	make clean
	make build

build:
	cmake -S . -B build
	cmake --build build

test_db_conn: build
	./build/test_db_conn

test_db_pool: build
	./build/test_db_conn

clean:
	rm -rf build/