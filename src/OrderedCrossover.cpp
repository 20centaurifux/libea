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
 * \file OrderedCrossover.cpp
 * \brief Implementation of ordered crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include "OrderedCrossover.h"
#include "RouteFactory.h"

using namespace std;
using namespace ea;

namespace ea
{
	uint32_t OrderedCrossover::crossover(const Individual* a, const Individual* b, vector<Individual*>& children)
	{
		uint32_t separator;
		uint32_t i;
		uint32_t m = 0;
		Individual *individual;

		ACROSSOVER_DEFAULT_ASSERT(a, b);

		separator = (uint32_t)generator->get_number(1, a->size() - 2);

		individual = new Individual(a->get_fitness_func(), a->size());

		for(i = 0; i < separator; i++)
		{
			individual->set_gene(i, a->gene_at(i)->clone());
		}

		for(i = separator; i < individual->size(); i++)
		{
			while(gene_exists(b->gene_at(m), individual, i))
			{
				m++;
			}

			assert(m < b->size());

			individual->set_gene(i, b->gene_at(m)->clone());
		}

		children.push_back(individual);

		return 1;
	}

	inline bool OrderedCrossover::gene_exists(Gene* gene, Individual* individual, const uint32_t len)
	{
		for(uint32_t i = 0; i < len; i++)
		{
			if(gene->equals(individual->gene_at(i)))
			{
				return true;
			}
		}

		return false;
	}
}
