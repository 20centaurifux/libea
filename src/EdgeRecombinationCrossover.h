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

#ifndef EDGERECOMBINATION_CROSSOVER_H
#define EDGERECOMBINATION_CROSSOVER_H

#include <cassert>
#include <cstring>
#include <memory>
#include <random>
#include "ACrossover.h"
#include "algorithms.h"
#include "ARandomNumberGenerator.h"
#include "TR1UniformDistribution.h"

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
	   @tparam TGenomeBase a genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the cycle crossover operator.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	class EdgeRecombinationCrossover : public ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/*! Gene datatype. */
			typedef typename TGenomeBase::gene_type gene_type;

			EdgeRecombinationCrossover()
			{
				_rnd = std::make_shared<TR1UniformDistribution<std::mt19937_64>>();
			}

			EdgeRecombinationCrossover(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
			{
				assert(set_equals<TGenomeBase>(a, b));

				Neighbors* nblist;
				std::map<gene_type, Neighbors*, LessThan> nbs;
				uint32_t count, len, i;
				int32_t index;

				len = _base.len(a);

				// initialize neighbor lists:
				nblist = new Neighbors[len];
				std::memset(nblist, 0, len * sizeof(Neighbors));

				for(i = 0; i < len; i++)
				{
					count = 0;

					add_neighbors(a, i, nblist[i].genes, count);
					index = _base.index_of(b, _base.get(a, i));
					add_neighbors(b, index, nblist[i].genes, count);

					nblist[i].count = count;
					nbs[_base.get(a, i)] = nblist + i;
				}

				// create child individual:
				gene_type x;
				Neighbors* n;
				Neighbors* next_nb;
				uint32_t next_count[5], j, child_i = 0;
				gene_type next_gene[5][5];

				auto child = _base.create(len);

				// get initial gene:
				x = _rnd->get_int32(0, 1) ? _base.get(a, 0) : _base.get(b, 0);

				for(;;)
				{
					// insert gene into child individual:
					_base.set(child, child_i,  x);

					// check if child is completed:
					if(++child_i == len)
					{
						break;
					}

					// remove x from all neighbor lists:
					n = nbs[x];

					for(j = 0; j < n->count; j++)
					{
						remove_neighbor(nbs[n->genes[j]], x);
					}

					// find next gene:
					if(n->count)
					{
						std::memset(next_count, 0, sizeof(next_count));

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
									x = next_gene[j][_rnd->get_int32(0, next_count[j] - 1)];
								}

								break;
							}
						}
					}
					else
					{
						do
						{
							x = _base.get(a, _rnd->get_int32(0,len - 1));
						} while(gene_exists(child, child_i, x));
					}
				}

				// append child:
				output.push(child);

				// free memory:
				delete[] nblist;

				return 1;
			}

		private:
			typedef struct
			{
				gene_type genes[4];
				uint32_t count;
			} Neighbors;

			static TGenomeBase _base;
			static LessThan _less_than;

			std::shared_ptr<ARandomNumberGenerator> _rnd;

			void add_neighbors(const sequence_type& individual, const uint32_t index, gene_type neighbors[4], uint32_t& count)
			{
				uint32_t len = _base.len(individual);

				if(index)
				{
					add_neighbor(neighbors, count, _base.get(individual, index - 1));
				}
				else
				{
					add_neighbor(neighbors, count, _base.get(individual, len - 1));

				}

				if(index == len - 1)
				{
					add_neighbor(neighbors, count, _base.get(individual, 0));
				}
				else
				{
					add_neighbor(neighbors, count, _base.get(individual, index + 1));

				}
			}

			void add_neighbor(gene_type neighbors[4], uint32_t& count, gene_type neighbor)
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
						if(!_less_than(neighbors[i], neighbor) && !_less_than(neighbor, neighbors[i]))
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

			void remove_neighbor(Neighbors* neighbors, const gene_type gene)
			{
				for(uint32_t i = 0; i < neighbors->count; i++)
				{
					if(!_less_than(neighbors->genes[i], gene) && !_less_than(gene, neighbors->genes[i]))
					{
						for(uint32_t m = i; m < neighbors->count - 1; m++)
						{
							neighbors->genes[m] = neighbors->genes[m + 1];
						}

						--neighbors->count;
						break;
					}
				}
			}

			inline bool gene_exists(const sequence_type& genome, const uint32_t size, const gene_type gene)
			{
				for(uint32_t i = 0; i < size; i++)
				{
					if(!_less_than(_base.get(genome, i), gene) && !_less_than(gene, _base.get(genome, i)))
					{
						return true;
					}
				}

				return false;
			}
	};

	template<typename TGenomeBase, typename LessThan>
	TGenomeBase EdgeRecombinationCrossover<TGenomeBase, LessThan>::_base;

	template<typename TGenomeBase, typename LessThan>
	LessThan EdgeRecombinationCrossover<TGenomeBase, LessThan>::_less_than;

	/**
		   @}
	   @}
	 */
}
#endif
