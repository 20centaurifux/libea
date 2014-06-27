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
   @file TwoPointCrossover.h
   @brief Implementation of the two-point crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef TWOPOINTCROSSOVER_H
#define TWOPOINTCROSSOVER_H

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
	   @class TwoPointCrossover
	   @tparam TGenome type of the genome class
	   @brief Implementation of the two-point crossover operator.
	 */
	template<typename TGenome>
	class TwoPointCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			TwoPointCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}

			virtual ~TwoPointCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				uint32_t offset0;
				uint32_t offset1;
				uint32_t max = a->size();

				if(max > b->size())
				{
					max = b->size();
				}

				assert(max >= 5);

				offset0 = (uint32_t)generator->get_int32(1, max - 3);
				offset1 = (uint32_t)generator->get_int32(offset0 + 1, max - 1);

				output.append(create_child(a, b, offset0, offset1));
				output.append(create_child(b, a, offset0, offset1));

				return 2;
			}

		protected:
			using ACrossover<TGenome>::generator;

		private:
			std::shared_ptr<TGenome> create_child(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, const uint32_t offset1, const uint32_t offset2) const
			{
				uint32_t i;
				std::shared_ptr<TGenome> individual;
			
				individual = std::make_shared<TGenome>(a->size(), a->get_fitness_func());

				for(i = 0; i < offset1; i++)
				{
					individual->copy_to(i, a->at(i));
				}

				for(i = offset1; i < offset2; i++)
				{
					individual->copy_to(i, b->at(i));
				}

				for(i = offset2; i < a->size(); i++)
				{
					individual->copy_to(i, a->at(i));
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
