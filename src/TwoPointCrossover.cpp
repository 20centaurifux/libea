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
 * \file TwoPointCrossover.cpp
 * \brief Implementation of two-point crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include "TwoPointCrossover.h"
#include "RouteFactory.h"

using namespace std;
using namespace ea;

namespace ea
{
	uint32_t TwoPointCrossover::crossover(const Individual* a, const Individual* b, vector<Individual*>& children)
	{
		uint32_t offset1;
		uint32_t offset2;

		ACROSSOVER_DEFAULT_ASSERT(a, b);

		offset1 = (uint32_t)generator->get_number(1, a->size() - 3);
		offset2 = (uint32_t)generator->get_number(offset1 + 1, a->size() - 1);

		children.push_back(crossover(b, a, offset1, offset2));
		children.push_back(crossover(a, b, offset1, offset2));

		return 2;
	}

	Individual* TwoPointCrossover::crossover(const Individual* a, const Individual* b, const uint32_t offset1, const uint32_t offset2) const
	{
		uint32_t i;
		Individual *individual;
	
		individual = new Individual(a->get_fitness_func(), a->size());

		for(i = 0; i < offset1; i++)
		{
			individual->set_gene(i, a->gene_at(i)->clone());
		}

		for(i = offset1; i < offset2; i++)
		{
			individual->set_gene(i, b->gene_at(i)->clone());
		}

		for(i = offset2; i < a->size(); i++)
		{
			individual->set_gene(i, a->gene_at(i)->clone());
		}

		return individual;
	}
}
