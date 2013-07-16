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
   @file DoubleSwapMutation.h
   @brief Mutation operator to swap three genes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef DOUBLESWAPMUTATION_H
#define DOUBLESWAPMUTATION_H

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
	   @class DoubleSwapMutation
	   @tparam TGenome type of the genome class.
	   @brief Swaps three genes.
	 */
	template<class TGenome>
	class DoubleSwapMutation : public AMutation<TGenome>
	{
		public:
			using AMutation<TGenome>::mutate;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			DoubleSwapMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : AMutation<TGenome>(rnd_generator) {}

			~DoubleSwapMutation() {};

			void mutate(std::shared_ptr<TGenome> genome) override
			{
				int32_t offsets[3];

				if(genome->size() <= 2)
				{
					return;
				}

				this->generator->get_unique_int32_seq(0, genome->size() - 1, offsets, 3);
				genome->swap(offsets[0], offsets[1]);
				genome->swap(offsets[1], offsets[2]);
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
