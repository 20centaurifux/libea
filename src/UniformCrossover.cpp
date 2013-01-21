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
/*!
 * \file UniformCrossover.h
 * \brief Implementation of uniform crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include "UniformCrossover.h"
#include "RouteFactory.h"

using namespace std;
using namespace ea;

namespace ea
{
	uint32_t UniformCrossover::crossover(const Individual* a, const Individual* b, vector<Individual*>& children)
	{
		Individual* child1;
		Individual* child2;
		int32_t rnd;

		ACROSSOVER_DEFAULT_ASSERT(a, b);

		child1 = new Individual(a->get_fitness_func(), a->size());
		child2 = new Individual(a->get_fitness_func(), a->size());

		for(uint32_t i = 0; i < a->size(); ++i)
		{
			do
			{
				rnd = generator->random();
			} while(!rnd);

			if(rnd % 2)
			{
				child1->copy_to(i, a->at(i));
				child2->copy_to(i, b->at(i));
			}
			else
			{
				child1->copy_to(i, b->at(i));
				child2->copy_to(i, a->at(i));
			}
		}

		children.push_back(child1);
		children.push_back(child2);

		return 2;
	}
}
