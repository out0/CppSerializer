all:
	rm -rf build
	mkdir -p build
	cd build && cmake ..
	cd build && make -j$(nproc)
	mv build/cpps .
	rm -rf build

clean:
	rm -rf build cpps




