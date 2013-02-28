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
   @file InverseBitStringMutation.h
   @brief Inverts all bits of the genome.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
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
	   @brief Inverts all bits of the genome.
	 */
	class InverseBitStringMutation : public ABinaryMutation
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			InverseBitStringMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ABinaryMutation(rnd_generator) {}

			void mutate(BinaryGenome* genome)
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
