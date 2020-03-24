//
// Created by ktulhu on 28/11/2018.
//

#ifndef PROJECT_SUPPORT_H
#define PROJECT_SUPPORT_H

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <numeric>
#include <fstream>

namespace prng {
	double rand_num(double min, double max);

	template <typename I>
	inline I random_element(I begin, I end) {
		unsigned long n = std::distance(begin, end);
		auto k = (unsigned int) ( std::rand() % n );
		std::advance(begin, k);
		return begin;
	}

	struct RandElement {
		// RAII random element structure for quicker weighting access
		explicit RandElement(const std::vector<double>& weighting)
			:	weighting{&weighting} {
		}

		template <typename I>
		I operator()(I begin){
			// using rand() cus it's a lot faster than the cpp generators
			// (obv less random, but for 1e10 cases it's totally fine)
			double k = ((double) rand() / (RAND_MAX));
			int index = 0;
			double sum = 0;
			for (const auto& i : *weighting) {
				sum += i;
				if (k < sum) {
					break;
				}
				++index;
			}
			// std::cout << "DEBUG index = " << index << std::endl;
			std::advance(begin, index);
			return begin;
		}

	private:
		const std::vector<double>* weighting;
	};

}

namespace sort {

	template <typename T>
	std::vector<int> get_permutation(const std::vector<T>& vec, const std::function<bool(T a, T b)>& compare) {
		std::vector<int> p(vec.size());
		std::iota(p.begin(), p.end(), 0);
		std::sort(p.begin(), p.end(), [&](int i, int j){ return compare(vec[i], vec[j]); });
		return p;
	}

	template <typename T>
	std::vector<T> apply_permutation(const std::vector<T>& vec, const std::vector<int>& p) {
		std::vector<T> sorted_vec(vec.size());
		std::transform(p.begin(), p.end(), sorted_vec.begin(), [&](int i){ return vec[i]; });
		return sorted_vec;
	}

}

namespace ptl{

	template <class T>
	void print(const std::vector<T>& v) {
		for (const auto& i : v) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}

}

#endif //PROJECT_SUPPORT_H
