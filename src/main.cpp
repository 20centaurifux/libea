﻿/***************************************************************************
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

#include "ea.h"

#include "AGene.h"
#include "SDBMHash.h"

#include <iostream>

using namespace ea;
using namespace std;

class City : public AGene
{
	public:
		City(const uint32_t x, const uint32_t y, const std::string name) : AGene(), _x(x), _y(y), _name(name) {}

		void set_name(const std::string& name)
		{
			_name = name;
			notifiy();
		}

		std::string get_name()
		{
			return _name;
		}

		uint32_t get_x()
		{
			return _x;
		}

		void set_x(const uint32_t x)
		{
			_x = x;
			notifiy();
		}

		uint32_t get_y()
		{
			return _y;
		}

		void set_y(const uint32_t y)
		{
			_y = y;
			notifiy();
		}

		bool equals(const AGene* object) const
		{
			const City* city = (City*)object;

			if((city = dynamic_cast<const City*>(object)))

			{
				return _x == city->_x && _y == city->_y && !_name.compare(city->_name);
			}

			return false;
		}

		City* clone() const
		{
			return new City(_x, _y, _name);
		}

		size_t hash() const
		{
			SDBMHash hash;

			hash << _x << _y << _name;

			return hash.hash();
		}

		std::string to_string()
		{
			return _name;
		}

	private:
		uint32_t _x;
		uint32_t _y;
		std::string _name;
};

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

	return f * -1;
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

template<class Vector>
void clear(Vector& vector)
{
	for(auto el : vector)
	{
		delete el;
	}

	vector.erase(vector.begin(), vector.end());
}

#define POP_SIZE 50
#define SEL_SIZE 40
#define ROUNDS   20

Genome* get_fittest(const vector<Genome*>& p)
{
	Genome* best = NULL;

	for(auto g : p)
	{
		if(!best || (best->fitness() <= g->fitness()))
		{
			best = g;
		}
	}

	return best;
}

int main()
{
	ARandomNumberGenerator* g = new AnsiRandomNumberGenerator();
	AIndexSelection<AGene*>* sel = new TournamentSelection<AGene*>(g);
	ACrossover<AGene*>* crossover = new EdgeRecombinationCrossover<AGene*, AGene::hash_func, AGene::equal_to>(g);
	AMutation<AGene*>* mutation = new DoubleSwapMutation<AGene*>(g);
	AFactory<Genome*>* factory = new RouteFactory(g, calculate_route);
	vector<Genome*> population;
	vector<Genome*> selection;
	vector<Genome*> children;

	// create initial population:
	population = factory->random(POP_SIZE);

	for(uint32_t i = 0; i < ROUNDS; ++i)
	{
		// select genomes:
		sel->select_population(population.begin(), population.end(), SEL_SIZE, selection);
		
		// crossover:
		crossover->multi_crossover(selection.begin(), selection.end(), children);

		// selection:
		clear(population);
		
		sel->select_population(children.begin(), children.end(), POP_SIZE, population);

		// mutation:
		mutation->multi_mutate(population.begin(), population.end(), 3);

		// show fittest genome:
		Genome* b = get_fittest(population);
		cout << "fittest genome: " << b->to_string() << ", " << b->fitness() * -1 << endl;

		clear(selection);
		clear(children);
	}

	// free memory:
	for_each(population.begin(), population.end(), [] (Genome *g) { delete g; });

	delete factory;
	delete mutation;
	delete crossover;
	delete sel;
	delete g;

	/*
	for_each(population.begin(), population.end(), [] (Genome* genome) { delete genome; });
	delete g;
	*/

	return 0;
}
