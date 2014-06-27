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
   @file OnePointCrossover.h
   @brief Implementation of the one-point crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ONEPOINTCROSSOVER_H
#define ONEPOINTCROSSOVER_H

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
	   @class OnePointCrossover
	   @tparam TGenome type of the genome class
	   @brief Implementation of the one-point crossover operator.
	 */
	template<typename TGenome>
	class OnePointCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			OnePointCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}

			virtual ~OnePointCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				uint32_t separator;
				uint32_t max = a->size();

				if(max > b->size())
				{
					max = b->size();
				}

				assert(max >= 5);
				separator = (uint32_t)generator->get_int32(1, max - 3);

				output.append(create_child(b, a, separator));
				output.append(create_child(a, b, separator));

				return 2;
			}

			using ACrossover<TGenome>::generator;

		private:
			std::shared_ptr<TGenome> create_child(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, const uint32_t separator) const
			{
				uint32_t i;
				std::shared_ptr<TGenome> individual;

				individual = std::make_shared<TGenome>(b->size(), b->get_fitness_func());

				for(i = 0; i < separator; i++)
				{
					individual->copy_to(i, a->at(i));
				}

				for(i = separator; i < b->size(); i++)
				{
					individual->copy_to(i, b->at(i));
				}

				return individual;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
