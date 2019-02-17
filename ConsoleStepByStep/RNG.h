#pragma once

#ifndef RNG_H
#define RNG_H

#include <random>
#include <ctime>

using namespace std;

int rng(int fromI, int toI)
{
	default_random_engine generator;
	uniform_int_distribution<int> distribution(fromI, toI);
	generator.seed(time(NULL));

	return distribution(generator);
}

#endif // RNG_H