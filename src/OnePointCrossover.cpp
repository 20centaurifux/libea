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
 * \file OnePointCrossover.h
 * \brief Implementation of one-point crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include "OnePointCrossover.h"
#include "RouteFactory.h"

using namespace std;
using namespace ea;

namespace ea
{
	uint32_t OnePointCrossover::crossover(const Individual* a, const Individual* b, vector<Individual*>& children)
	{
		uint32_t separator;

		ACROSSOVER_DEFAULT_ASSERT(a, b);
	
		separator = (uint32_t)generator->get_number(1, a->size() - 2);

		children.push_back(crossover(b, a, separator));
		children.push_back(crossover(a, b, separator));

		return 2;
	}

	Individual* OnePointCrossover::crossover(const Individual* a, const Individual* b, const uint32_t separator) const
	{
		uint32_t i;
		Individual *individual;
	
		individual = new Individual(a->get_fitness_func(), a->size());

		for(i = 0; i < separator; i++)
		{
			individual->copy_to(i, a->at(i));
		}

		for(i = separator; i < a->size(); i++)
		{
			individual->copy_to(i, b->at(i));
		}

		return individual;
	}
}
