/***************************************************************************
    begin........: December 2012
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
 * \file CycleCrossover.cpp
 * \brief Implementation of cycle crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include "CycleCrossover.h"
#include "RouteFactory.h"
#include "foreach.h"

using namespace std;
using namespace ea;

namespace ea
{
	uint32_t CycleCrossover::crossover(const Individual* a, const Individual* b, vector<Individual*>& children)
	{
		int32_t index = 0;
		uint32_t offset = 1;
		Gene* gene;
		vector<Gene*> *cycle;
		int32_t count = 1;
		vector<vector<Gene*>*> cycles;
		bool flag = true;
		Individual *child1;
		Individual *child2;
		Individual *p1;
		Individual *p2;

		ACROSSOVER_DEFAULT_ASSERT(a, b);

		// create initial cycle:
		cycles.push_back((cycle = new vector<Gene*>()));

		// insert first gene of individual "a" into initial cycle:
		cycle->push_back(a->gene_at(0));

		// find next gene:
		gene = next_gene(a, b, index);

		// check if we have inserted all exisiting genes:
		while(count != (int32_t)a->size())
		{
			// has current gene already been assigned?
			while(!gene_assigned(cycles, gene))
			{
				// insert current gene into current cycle & update counter:
				cycle->push_back(gene);
				count++;

				// find next gene:
				gene = next_gene(a, b, index);
			}

			// check if we have inserted all exisiting genes:
			if(count != (int32_t)a->size())
			{
				// find next unassigned gene:
				while(offset < a->size() - 1 && gene_assigned(cycles, a->gene_at(offset)))
				{
					offset++;
				}
	
				gene = a->gene_at((index = offset++));

				// create new cycle:
				cycles.push_back((cycle = new vector<Gene*>()));
			}
		}

		// create children:
		child1 = new Individual(a->get_fitness_func(), a->size());
		child2 = new Individual(a->get_fitness_func(), a->size());

		for(uint32_t i = 0; i < cycles.size(); i++)
		{
			cycle = cycles.at(i);

			if(flag)
			{
				p1 = child1;
				p2 = child2;
			}
			else
			{
				p1 = child2;
				p2 = child1;
			}

			for(uint32_t m = 0; m < a->size(); m++)
			{
				if(IEquatable<Gene>::find(*cycle, a->gene_at(m)) != -1)
				{
					p1->set_gene(m, a->gene_at(m)->clone());
				}

				if(IEquatable<Gene>::find(*cycle, b->gene_at(m)) != -1)
				{
					p2->set_gene(m, b->gene_at(m)->clone());
				}
			}

			flag = !flag;
		}

		children.push_back(child1);
		children.push_back(child2);

		// free memory:
		foreach<vector<Gene*>*>(cycles, delete_object<vector<Gene*> >);

		return 2;
	}
}
