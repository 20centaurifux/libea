/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: sebastian.fedrau@gmail.com
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
 ***************************************************************************/
/**
   @file EdgeRecombinationCrossover.h
   @brief Implementation of the edge recombination crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef EDGERECOMBINATIONCROSSOVER_H
#define EDGERECOMBINATIONCROSSOVER_H

#include <cassert>
#include <cstring>
#include <map>
#include "ACrossover.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Crossover
		@{
	 */

	/**
	   @class EdgeRecombinationCrossover
	   @tparam TGenome type of the genome class
	   @tparam LessThan functor to compare genes
	   @brief Implementation of the edge recombination crossover operator.
	 */
	template<typename TGenome, typename LessThan = std::less<typename TGenome::value_type>>
	class EdgeRecombinationCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			EdgeRecombinationCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}

			virtual ~EdgeRecombinationCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				Neighbors* nblist;
				std::map<T, Neighbors*, LessThan> nbs;
				uint32_t count;
				T x;
				std::shared_ptr<TGenome> child;
				uint32_t child_i = 0;
				uint32_t i, j, index;
				Neighbors* n;
				Neighbors* next_nb;
				uint32_t next_count[5];
				T next_gene[5][5];

				assert(a->size() == b->size());

				// initialize neighbor lists:
				nblist = new Neighbors[a->size()];
				std::memset(nblist, 0, a->size() * sizeof(Neighbors));

				for(i = 0; i < a->size(); ++i)
				{
					count = 0;

					add_neighbors(a, i, nblist[i].genes, count);
					b->index_of(a->at(i), index);
					add_neighbors(b, index, nblist[i].genes, count);

					nblist[i].count = count;
					nbs[a->at(i)] = nblist + i;
				}

				// create child individual:
				child = std::make_shared<TGenome>(a->size(), a->get_fitness_func());

				// get initial gene:
				x = this->generator->get_int32(0, 1) ? a->at(0) : b->at(0);

				for(;;)
				{
					// insert gene into child individual:
					child->copy_to(child_i, x);

					// check if child is completed:
					if(++child_i == a->size())
					{
						break;
					}

					// remove x from all neighbor lists:
					n = nbs[x];

					for(j = 0; j < n->count; ++j)
					{
						remove_neighbor(nbs[n->genes[j]], x);
					}

					// find next gene:
					if(n->count)
					{
						memset(next_count, 0, sizeof(next_count));

						for(j = 0; j < n->count; j++)
						{
							next_nb = nbs[n->genes[j]];
							next_gene[next_nb->count][next_count[next_nb->count]++] = n->genes[j];
						}

						for(j = 4; j >= 0; j--)
						{
							if(next_count[j] || !j)
							{
								if(next_count[j] == 1)
								{
									x = next_gene[j][0];
								}
								else
								{
									x = next_gene[j][this->generator->get_int32(0, next_count[j] - 1)];
								}

								break;
							}
						}
					}
					else
					{
						do
						{
							x = a->at(this->generator->get_int32(0, a->size() - 1));
						} while(gene_exists(child, child_i, x));
					}
				}

				// append child:
				output.append(child);

				// free memory:
				delete[] nblist;

				return 1;
			}

		private:
			typedef typename TGenome::value_type T;

			typedef struct
			{
				T genes[4];
				uint32_t count;
			} Neighbors;

			LessThan _lessthan;

			void add_neighbors(const std::shared_ptr<TGenome> &individual, const uint32_t index, T neighbors[4], uint32_t &count)
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
					for(uint32_t i = 0; i < count; ++i)
					{
						if(!_lessthan(neighbors[i], neighbor) && !_lessthan(neighbor, neighbors[i]))
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
					if(!_lessthan(neighbors->genes[i], gene) && !_lessthan(gene, neighbors->genes[i]))
					{
						for(uint32_t m = i; m < neighbors->count - 1; ++m)
						{
							neighbors->genes[m] = neighbors->genes[m + 1];
						}

						--neighbors->count;
						break;
					}
				}
			}

			inline bool gene_exists(const std::shared_ptr<TGenome> &genome, const uint32_t size, const T gene)
			{
				for(uint32_t i = 0; i < size; ++i)
				{
					if(!_lessthan(genome->at(i), gene) && !_lessthan(gene, genome->at(i)))
					{
						return true;
					}
				}

				return false;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
