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
   @file SingleSwapMutation.h
   @brief Mutation operator to swap two genes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef SINGLESWAPMUTATION_H
#define SINGLESWAPMUTATION_H

#include "AMutation.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class SingleSwapMutation
	   @tparam TGenome type of the genome class.
	   @brief Swaps two genes.
	 */
	template<class TGenome>
	class SingleSwapMutation : public AMutation<TGenome>
	{
		public:
			using AMutation<TGenome>::mutate;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			SingleSwapMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : AMutation<TGenome>(rnd_generator) {}

			~SingleSwapMutation() {};

			void mutate(std::shared_ptr<TGenome> genome) override
			{
				int32_t offsets[2];

				if(genome->size() <= 1)
				{
					return;
				}

				this->generator->get_unique_int32_seq(0, genome->size() - 1, offsets, 2);
				genome->swap(offsets[0], offsets[1]);
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
