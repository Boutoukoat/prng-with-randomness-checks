
all: ./prng_example

./prng_example: prng_example.cpp
	g++ -O3 -march=native -o prng_example prng_example.cpp

tests: ./prng_example
	@echo ""
	@echo "---------------------------------------------------------"
	@echo "Generate a file about 1 GB"
	@echo ""
	./prng_example prng_example.bin
	@echo ""
	@echo "---------------------------------------------------------"
	@echo "Test the file"
	@echo ""
	dieharder -a -g 201 -f prng_example.bin

clean: 
	rm -f ./prng_example.bin ./prng_example
