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
   @file OrderedCrossover.h
   @brief Implementation of the ordered crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ORDEREDCROSSOVER_H
#define ORDEREDCROSSOVER_H

#include <cassert>
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
	   @class OrderedCrossover
	   @tparam TGenome type of the genome class
	   @tparam Equals optional functor to compare genes
	   @brief Implementation of the ordered crossover operator.
	 */
	template<typename TGenome,  typename LessThan = std::less<typename TGenome::value_type>>
	class OrderedCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			OrderedCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}

			virtual ~OrderedCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				uint32_t separator;
				uint32_t i;
				uint32_t m = 0;
				uint32_t size = a->size();
				std::shared_ptr<TGenome> individual;

				if(size > b->size())
				{
					size = b->size();
				}

				assert(size >= 4);

				separator = (uint32_t)generator->get_int32(1, size - 2);
				individual = std::make_shared<TGenome>(size, a->get_fitness_func());

				for(i = 0; i < separator; i++)
				{
					individual->copy_to(i, a->at(i));
				}

				for(i = separator; i < size; i++)
				{
					while(gene_exists(b->at(m), individual, i))
					{
						m++;
						assert(m < b->size());
					}

					individual->copy_to(i, b->at(m));
				}

				output.append(individual);

				return 1;
			}

		protected:
			using ACrossover<TGenome>::generator;

		private:
			typedef typename TGenome::value_type T;

			LessThan less_than;

			inline bool gene_exists(const T gene, const std::shared_ptr<TGenome> &individual, const uint32_t len)
			{
				for(uint32_t i = 0; i < len; ++i)
				{
					if(!less_than(gene, individual->at(i)) && !less_than(individual->at(i), gene))
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
