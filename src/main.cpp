/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/**
   @file main.cpp
   @brief Contains some testing stuff.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

#include "ea.h"

#include "City.h"
#include "AFactory.h"

using namespace ea;
using namespace std;

float calculate_route(const AGenome<AGene*>& individual)
{
	City* city;
	float x0, x1, y0, y1;
	float f = 0;

	for(uint32_t i = 0; i < individual.size(); ++i)
	{
		city = dynamic_cast<City*>(individual.at(i));

		if(!i)
		{
			x0 = city->get_x();
			y0 = city->get_y();
		}
		else
		{
			x1 = city->get_x();
			y1 = city->get_y();

			f += sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));

			x0 = city->get_x();
			y0 = city->get_y();
		}
	}

	return f;
}

class RouteFactory : public AFactory<Genome*>
{
	public:
		static const int32_t N_CITIES = 20;

		RouteFactory(ARandomNumberGenerator* rnd_generator,  FitnessFunc<AGene*>::fitness fitness_func)
			: AFactory<Genome*>(rnd_generator), _fitness_func(fitness_func) {};
		virtual ~RouteFactory() {};

		vector<Genome*> random(const uint32_t count)
		{
			vector<Genome*> population;
			int32_t numbers[N_CITIES];
			Genome* individual;

			for(uint32_t i = 0; i < count; ++i)
			{
				individual = new Genome(N_CITIES, _fitness_func);
				AFactory<Genome*>::generator->get_unique_numbers(0, N_CITIES - 1, numbers, N_CITIES);

				for(int32_t j = 0; j < N_CITIES; j++)
				{
					individual->set(j, RouteFactory::create_new_city(numbers[j]));
				}

				population.push_back(individual);
			}

			return population;
		}

	private:
		FitnessFunc<AGene*>::fitness _fitness_func;
		static const uint32_t _points[N_CITIES][2];

		static City* create_new_city(const int32_t n)
		{
			char name[16];

			memset(name, 0, 16);
			sprintf(name, "%c", n + 65);

			return new City(_points[n][0], _points[n][1], name);
		}
};

const uint32_t RouteFactory::_points[20][2] =
{
	{ 12, 104 }, { 8234, 1800 }, { 929, 523 }, { 638, 102 }, { 72, 4123 },
	{ 7921, 2331 }, { 1902, 138 }, { 231, 9881}, { 97, 7510 }, { 711, 8391 },
	{ 96, 7313 }, { 219, 6 }, { 2, 8190 }, { 1532, 2310 }, { 5, 9 },
	{ 136, 196 }, { 54, 6377 }, { 74, 14 }, { 1569, 736 }, { 175, 1841 }
};

/*
float calculate_route(const AGenome<AGene*>& foo)
{
	return 0;
}
*/
	   
//Genome(const uint32_t size, const typename FitnessFunc<AGene<T>*>::fitness fitness_func) :
//typedef float (*fitness)(const AGenome<T>& obj);

//FitnessFunc<AGene<City>*> foobar = calculate_route;

void print_cities(AGenome<AGene*>* individual)
{
	City* city;

	for(uint32_t i = 0; i < individual->size(); ++i)
	{
		city = (City*)individual->at(i);
		cout << city->get_name().c_str();

		if(i <= individual->size() - 2)
		{
			cout << ", ";
		}
	}

	cout << endl;
}

float fitness(const AGenome<int>& g)
{
	float sum = 0;

	for(uint32_t i = 0; i < g.size(); ++i)
	{
		sum += g.at(i);
	}

	return sum / 100;
}

void print_binary_genome(const BinaryGenome& genome)
{
	for(uint32_t i = 0; i < genome.size(); ++i)
	{
		cout << (genome.at(i) ? "1" : "0");
	}

	cout << endl;
}

int main()
{
	ARandomNumberGenerator* g = new MersenneTwisterUniformIntDistribution();

	/*
	RouteFactory f = RouteFactory(g, calculate_route);

	vector<Genome*> parents;

	parents = f.random(2);

	print_cities(parents.at(0));
	print_cities(parents.at(1));

	EdgeRecombinationCrossover<AGene*, AGene::hash_func, AGene::equal_to> c(g);

	vector<AGenome<AGene*>*> children;

	c.crossover(parents.at(0), parents.at(1), children);

	print_cities(children.at(0));
	//print_cities(children.at(1));

	cout << parents.at(0)->fitness() << endl;

	SingleSwapMutation<AGene*> m(g);

	m.mutate(children.at(0));

	print_cities(children.at(0));

	cout << children.at(0)->fitness() << endl;

	for_each(parents.begin(), parents.end(), [] (Genome* genome) { delete genome; });
	for_each(children.begin(), children.end(), [] (AGenome<AGene*>* genome) { delete genome; });
	*/

	vector<AGenome<int>*> population;

	for(uint32_t i = 0; i < 30; ++i)
	{
		AGenome<int>* genome = new PrimitiveGenome<int>(8, fitness);

		for(uint32_t m = 0; m < 8; ++m)
		{
			genome->set(m, g->get_number(1, 100));
		}

		population.push_back(genome);
	}

	float foo = 0;

	for(uint32_t i = 0; i < 30; ++i)
	{
		cout << "population " << i << ": " << population.at(i)->fitness() << endl;
		foo += population.at(i)->fitness();
	}

	cout << foo / population.size() << endl;

	vector<uint32_t> selection;

	FitnessProportionalSelection<int> sel(g);

	sel.select(population, 15, selection);

	cout << endl;

	foo = 0;

	for(uint32_t i = 0; i < selection.size(); ++i)
	{
		cout << "population " << selection[i] << ": " << population.at(selection[i])->fitness() << endl;
		foo += population.at(selection[i])->fitness();
	}

	cout << foo / selection.size() << endl;

	delete g;

	return 0;
}
