//
// Created by ktulhu on 28/11/2018.
//

#ifndef PROJECT_BENCHMARK_H
#define PROJECT_BENCHMARK_H

#include <ctime>
#include <iostream>

class Timeit {
public:
	Timeit() {
		start = std::clock();
	}

	~Timeit() {
		std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
	}
private:
	std::clock_t start;
};

#endif //PROJECT_BENCHMARK_H
