bin:
	rm -rf build
	mkdir -p build
	cd build && cmake ..
	cd build && make -j$(nproc)

all: bin 	
	mv build/unittest .
	mv build/tst_client .
	mv build/tst_server .
	mv build/tst_bridge .
	rm -rf build

test: all
	./unittest
	rm unittest

install: bin
	cd build && sudo make install

clean:
	rm -rf build unittest




