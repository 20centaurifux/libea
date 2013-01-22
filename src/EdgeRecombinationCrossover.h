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
 * \file EdgeRecombinationCrossover.h
 * \brief Implementation of edge recombination crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef EDGERECOMBINATIONCROSSOVER_H
#define EDGERECOMBINATIONCROSSOVER_H

#include <functional>
#include <map>
#include <cstring>
#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	template<class T, class Hash, class Equals = std::equal_to<T> >
	class EdgeRecombinationCrossover : public ACrossover
	{
		public:
			EdgeRecombinationCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover(rnd_generator) {}
			virtual ~EdgeRecombinationCrossover() {};

			uint32_t crossover(const ea::Individual* a, const ea::Individual* b, std::vector<Individual*>& children)
			{
				Neighbors nblist[a->size()];
				std::map<size_t, Neighbors*> nbs;
				uint32_t count;
				T x;
				Individual* child;
				uint32_t child_i = 0;
				uint32_t i, j;
				Neighbors* n;
				Neighbors* next_nb;
				uint32_t next_count[5];
				T next_gene[5][5];

				// initialize neighbor lists:
				std::memset(nblist, 0, sizeof(nblist));

				for(i = 0; i < a->size(); ++i)
				{
					count = 0;

					add_neighbors(a, i, nblist[i].genes, count);
					add_neighbors(b, b->index_of(a->at(i)), nblist[i].genes, count);

					nblist[i].count = count;
					nbs[hash(a->at(i))] = nblist + i;
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
					n = nbs[hash(x)];

					for(j = 0; j < n->count; ++j)
					{
						remove_neighbor(nbs[hash(n->genes[j])], x);
					}

					// find next gene:
					if(n->count)
					{
						memset(next_count, 0, sizeof(next_count));

						for(j = 0; j < n->count; ++j)
						{
							next_nb = nbs[hash(n->genes[j])];
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

		private:
			typedef struct
			{
				T genes[4];
				uint32_t count;
			} Neighbors;

			Equals equals;
			Hash hash;

			void add_neighbors(const ea::Individual* individual, const uint32_t index, T neighbors[4], uint32_t& count)
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

			void add_neighbor(T neighbors[4], uint32_t& count, T neighbor)
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
						if(equals(neighbors[i], neighbor))
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

			void remove_neighbor(Neighbors* neighbors, const T gene)
			{
				for(uint32_t i = 0; i < neighbors->count; ++i)
				{
					if(equals(neighbors->genes[i], gene))
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

			inline bool gene_exists(const Genome* genome, const uint32_t size, const T gene)
			{
				for(uint32_t i = 0; i < size; ++i)
				{
					if(equals(genome->at(i), gene))
					{
						return true;
					}
				}

				return false;
			}
	};
}
#endif
