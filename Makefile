all: run1 run2
	
run1: calc_integer.cpp
	g++ -std=c++11 calc_integer.cpp -o 3_4_1.out

run2: calc_scientific.cpp
	g++ -std=c++11 calc_scientific.cpp -o 3_4_2.out

clean: 
	rm *.out