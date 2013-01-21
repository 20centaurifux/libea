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
 * \file EdgeRecombinationCrossover.cpp
 * \brief Implementation of edge recombination crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <map>
#include <cstring>
#include "EdgeRecombinationCrossover.h"

using namespace std;

namespace ea
{
	uint32_t EdgeRecombinationCrossover::crossover(const Individual* a, const Individual* b, vector<Individual*>& children)
	{
		Neighbors nblist[a->size()];
		map<size_t, Neighbors*> nbs;
		uint32_t count;
		Gene* x;
		Individual* child;
		uint32_t child_i = 0;
		uint32_t i, j;
		Neighbors* n;
		Neighbors* next_nb;
		uint32_t next_count[5];
		Gene* next_gene[5][5];

		// initialize neighbor lists:
		memset(nblist, 0, sizeof(nblist));

		for(i = 0; i < a->size(); ++i)
		{
			count = 0;

			add_neighbors(a, i, nblist[i].genes, count);
			add_neighbors(b, b->index_of(a->at(i)), nblist[i].genes, count);

			nblist[i].count = count;
			nbs[a->at(i)->hash()] = nblist + i;
		}

		// create child individual:
		child = new Individual(a->get_fitness_func(), a->size());

		// get initial gene:
		x = generator->get_number(0, 1) ? a->at(0) : b->at(0);

		while(1)
		{
			// insert gene into child individual:
			child->copy_to(child_i, x);

			// check if child is completed:
			if(++child_i == a->size())
			{
				break;
			}

			// remove x from all neighbor lists:
			n = nbs[x->hash()];

			for(j = 0; j < n->count; ++j)
			{
				remove_neighbor(nbs[n->genes[j]->hash()], x);
			}

			// find next gene:
			if(n->count)
			{
				memset(next_count, 0, sizeof(next_count));

				for(j = 0; j < n->count; ++j)
				{
					next_nb = nbs[n->genes[j]->hash()];
					next_gene[next_nb->count][next_count[next_nb->count]++] = n->genes[j];
				}

				for(j = 4; j >= 0; --j)
				{
					if(next_count[j] || !j)
					{
						x = next_gene[j][generator->get_number(0, next_count[j] - 1)];
						break;
					}
				}
			}
			else
			{
				do
				{
					x = a->at(generator->get_number(0, a->size() - 1));
				} while(gene_exists(child, child_i, x));
			}
		}

		// append child:
		children.push_back(child);

		return 1;
	}

	void EdgeRecombinationCrossover::add_neighbors(const ea::Individual* individual, const uint32_t index, Gene* neighbors[4], uint32_t& count) const
	{
		if(index)
		{
			add_neighbor(neighbors, count, individual->at(index - 1));
		}
		else
		{
			add_neighbor(neighbors, count, individual->at(individual->size() - 1));

		}

		if(index == individual->size() - 1)
		{
			add_neighbor(neighbors, count, individual->at(0));
		}
		else
		{
			add_neighbor(neighbors, count, individual->at(index + 1));

		}
	}

	void EdgeRecombinationCrossover::add_neighbor(Gene* neighbors[4], uint32_t& count, Gene* neighbor) const
	{
		bool found = false;

		if(!count)
		{
			neighbors[0] = neighbor;
			count++;
		}
		else
		{
			for(uint32_t i = 0; i < count; i++)
			{
				if(neighbors[i]->equals(neighbor))
				{
					found = true;
					break;
				}
			}

			if(!found)
			{
				neighbors[count++] = neighbor;
			}
		}
	}

	void EdgeRecombinationCrossover::remove_neighbor(Neighbors* neighbors, const Gene* gene) const
	{
		for(uint32_t i = 0; i < neighbors->count; ++i)
		{
			if(neighbors->genes[i]->equals(gene))
			{
				for(uint32_t m = i; m < neighbors->count - 1; ++m)
				{
					neighbors->genes[m] = neighbors->genes[m + 1];
				}

				neighbors->genes[--neighbors->count] = NULL;
				break;
			}
		}
	}

	inline bool EdgeRecombinationCrossover::gene_exists(const Genome* genome, const uint32_t size, const Gene* gene) const
	{
		for(uint32_t i = 0; i < size; ++i)
		{
			if(genome->at(i)->equals(gene))
			{
				return true;
			}
		}

		return false;
	}
}
