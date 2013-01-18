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
 * \file PMXCrossover.cpp
 * \brief Implementation of PMX crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <cassert>
#include "PMXCrossover.h"

using namespace std;

namespace ea
{
	uint32_t PMXCrossover::crossover(const Individual* a, const Individual* b, vector<Individual*>& children)
	{
		uint32_t offset1;
		uint32_t offset2;
		Individual* child;

		ACROSSOVER_DEFAULT_ASSERT(a, b);

		offset1 = 0;
		offset2 = 10;

		child = crossover(a, b, offset1, offset2);
		children.push_back(child);

		child = crossover(b, a, offset1, offset2);
		children.push_back(child);

		return 2;
	}

	Individual* PMXCrossover::crossover(const Individual* parent1, const Individual* parent2, const uint32_t offset1, const uint32_t offset2) const
	{
		Individual* child;
		uint32_t i;
		int32_t index;
		Gene* gene;
		Gene* gene_p1;
		Gene* gene_p2;

		child = new Individual(parent1->get_fitness_func(), parent1->size());

		for(i = offset1; i <= offset2; ++i)
		{
			child->set_gene(i, parent1->gene_at(i)->clone());
		}

		for(i = offset1; i <= offset2; ++i)
		{
			gene = parent2->gene_at(i);

			if(child->find_gene(gene) == -1)
			{
				gene_p2 = gene;

				while(1)
				{
					index = parent2->find_gene(gene_p2);
					gene_p1 = parent1->gene_at(index);
					index = parent2->find_gene(gene_p1);

					if(index >= offset1 && index <= offset2)
					{
						gene_p2 = gene_p1;
					}
					else
					{
						child->set_gene(index, gene->clone());
						break;
					}
				}
			}
		}

		index = 0;

		for(i = 0; i < offset1; ++i)
		{
			if(child->find_gene((gene = parent1->gene_at(i))) == -1)
			{
				next_index(child, index);
				child->set_gene(index, gene->clone());
			}

		}

		for(i = offset2 + 1; i < child->size(); ++i)
		{
			if(child->find_gene((gene = parent1->gene_at(i))) == -1)
			{
				next_index(child, index);
				child->set_gene(index, gene->clone());
			}
		}

		return child;
	}
}
