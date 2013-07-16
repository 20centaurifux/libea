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
   @file PMXCrossover.h
   @brief Implementation of the PMX crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef PMXCROSSOVER_H
#define PMXCROSSOVER_H

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
	   @class PMXCrossover
	   @tparam TGenome type of the genome class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the PMX crossover operator.
	 */
	template<typename TGenome, typename LessThan = std::less<typename TGenome::value_type>>
	class PMXCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			PMXCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}

			virtual ~PMXCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				uint32_t offset0;
				uint32_t offset1;

				offset0 = generator->get_int32(0, a->size() - 3);
				offset1 = generator->get_int32(offset0 + 1, a->size() - 1);

				output.append(crossover(a, b, offset0, offset1));
				output.append(crossover(b, a, offset0, offset1));

				return 2;
			}

		protected:
			using ACrossover<TGenome>::generator;

		private:
			typedef typename TGenome::value_type T;

			std::shared_ptr<TGenome>crossover(const std::shared_ptr<TGenome> &parent0, const std::shared_ptr<TGenome> parent1, const uint32_t offset0, const uint32_t offset1) const
			{
				uint32_t i;
				uint32_t index;
				T gene;
				T g0;
				T g1;
				std::map<T, bool, LessThan> assigned; // a cache to test if a gene is assigned to the child genome
				bool* indices;                        // array used to store the information if an index is in use or not
				uint32_t size = parent0->size();
				uint32_t start = 0;
				std::shared_ptr<TGenome> child;

				indices = new bool[size];

				std::fill(indices, indices + size, false);
				std::fill(indices + offset0, indices + offset1 + 1, true);

				child = std::make_shared<TGenome>(size, parent0->get_fitness_func());

				for(i = 0; i < size; i++)
				{
					assigned[parent0->at(i)] = false;
				}

				for(i = offset0; i <= offset1; i++)
				{
					child->copy_to(i, parent0->at(i));
					assigned[parent0->at(i)] = true;
				}

				for(i = offset0; i <= offset1; i++)
				{
					gene = parent1->at(i);

					if(!assigned[gene])
					{
						g1 = gene;

						for(;;)
						{
							parent1->index_of(g1, index);
							g0 = parent0->at(index);
							parent1->index_of(g0, index);

							if(index >= offset0 && index <= offset1)
							{
								g1 = g0;
							}
							else
							{
								child->copy_to(index, gene);
								assigned[gene] = indices[index] = true;
								break;
							}
						}
					}
				}

				index = next_unassigned(indices, 0, size), start++;

				for(i = 0; i < offset0; ++i)
				{
					if(!assigned[(gene = parent0->at(i))])
					{
						child->copy_to(index, gene);
						assigned[gene] = indices[index] = true;
						index = next_unassigned(indices, start++, size);
					}
				}

				for(i = offset1 + 1; i < size; i++)
				{
					if(!assigned[(gene = parent0->at(i))])
					{
						child->copy_to(index, gene);
						assigned[gene] = indices[index] = true;
						index = next_unassigned(indices, start++, size);
					}
				}

				delete[] indices;

				return child;
			}

			inline uint32_t next_unassigned(const bool* indices, const uint32_t first, const uint32_t last) const
			{
				for(uint32_t i = first; i < last; ++i)
				{
					if(!indices[i])
					{
						return i;
					}
				}

				return 0;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
