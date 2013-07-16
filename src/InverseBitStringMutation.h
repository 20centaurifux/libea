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
   @file BitStringMutation.h
   @brief Inverts all bits of a binary genome.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef INVERSEBITSTRINGMUTATION_H
#define INVERSEBITSTRINGMUTATION_H

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
	   @class InverseBitStringMutation
	   @brief Inverts all bits of a binary genome.
	 */
	class InverseBitStringMutation : public ABinaryMutation
	{
		public:
			using ABinaryMutation::mutate;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			InverseBitStringMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ABinaryMutation(rnd_generator) {}

			/**
			   @param genome genome to mutate
			 
			   Mutates a genome.
			 */
			void mutate(std::shared_ptr<PrimitiveGenome<bool>> genome) override
			{
				for(uint32_t i = 0; i < genome->size(); ++i)
				{
					genome->flip(i);
				}
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
