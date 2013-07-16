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
   @file SingleBitStringMutation.h
   @brief Flips a single bit of a genome.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef SINGLEBITSTRINGMUTATION_H
#define SINGLEBITSTRINGMUTATION_H

#include "ABinaryMutation.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class SingleBitStringMutation
	   @brief Flips a single bit of a genome.
	 */
	class SingleBitStringMutation : public ABinaryMutation
	{
		public:
			using ABinaryMutation::mutate;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			SingleBitStringMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ABinaryMutation(rnd_generator) {}

			/**
			   @param genome genome to mutate
			 
			   Mutates a genome.
			 */
			void mutate(std::shared_ptr<PrimitiveGenome<bool>> genome) override
			{
				genome->flip(this->generator->get_int32(0, genome->size() - 1));
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
