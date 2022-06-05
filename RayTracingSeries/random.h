#pragma once
#include <random>

inline double random() {
	return rand() / (RAND_MAX + 1.0);
}
