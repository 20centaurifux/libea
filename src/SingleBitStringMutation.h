/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/**
   @file SingleBitStringMutation.h
   @brief Flipping a single bit in the genome.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
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
	   @brief Flipping a single bit in the genome.
	 */
	class SingleBitStringMutation : public ABinaryMutation
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			SingleBitStringMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ABinaryMutation(rnd_generator) {}

			void mutate(BinaryGenome* genome)
			{
				genome->flip(generator->get_number(0, genome->size() - 1));
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
