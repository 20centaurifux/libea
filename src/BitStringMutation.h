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
   @file BitStringMutation.h
   @brief Flipping bits with a probability of 1 / length of the genome.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef BITSTRINGMUTATION_H
#define BITSTRINGMUTATION_H

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
	   @class BitStringMutation
	   @brief Flipping bits with a probability of 1 / length of the genome.
	 */
	class BitStringMutation : public ABinaryMutation
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			BitStringMutation(ARandomNumberGenerator* rnd_generator) : ABinaryMutation(rnd_generator) {}

			void mutate(BinaryGenome* genome)
			{
				bool flipped = false;

				while(!flipped)
				{
					for(uint32_t i = 0; i < genome->size(); ++i)
					{
						if(!generator->get_number(0, genome->size() - 1))
						{
							genome->flip(i);
							flipped = true;
						}
					}
				}
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
