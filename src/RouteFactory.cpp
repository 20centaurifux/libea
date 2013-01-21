#include <cstring>
#include <cstdio>
#include "RouteFactory.h"

using namespace std;
using namespace ea;

const uint32_t RouteFactory::_points[20][2] =
{
	{ 12, 104 }, { 8234, 1800 }, { 929, 523 }, { 638, 102 }, { 72, 4123 },
	{ 7921, 2331 }, { 1902, 138 }, { 231, 9881}, { 97, 7510 }, { 711, 8391 },
	{ 96, 7313 }, { 219, 6 }, { 2, 8190 }, { 1532, 2310 }, { 5, 9 },
	{ 136, 196 }, { 54, 6377 }, { 74, 14 }, { 1569, 736 }, { 175, 1841 }
};

RouteFactory::RouteFactory(ARandomNumberGenerator* rnd_generator, FitnessFunc fitness) : AFactory<Individual*>(rnd_generator)
{
	_fitness_func = fitness;
}

City* RouteFactory::create_new_city(const int32_t n)
{
	char name[16];

	memset(name, 0, 16);
	sprintf(name, "%c", n + 65);

	return new City(_points[n][0], _points[n][1], name);
}

vector<Individual*> RouteFactory::random(const uint32_t count)
{
	vector<Individual*> population;
	int32_t numbers[N_CITIES];
	Individual* individual;

	for(uint32_t i = 0; i < count; i++)
	{
		individual = new Individual(_fitness_func, N_CITIES);
		generator->get_unique_numbers(0, N_CITIES - 1, numbers, N_CITIES);

		for(int32_t j = 0; j < N_CITIES; j++)
		{
			individual->set(j, create_new_city(numbers[j]));
		}

		population.push_back(individual);
	}

	return population;
}
