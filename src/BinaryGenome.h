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
   @file BinaryGenome.h
   @brief A genome representing a binary string.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#include "PrimitiveGenome.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class BinaryGenome
	   @brief A genome representing a binary string.
	 */
	class BinaryGenome : public PrimitiveGenome<bool>
	{
		public:
			/**
			   @param size size of the genome
			   @param fitness_func functor to calculate the fitness of the genome
			 */
			BinaryGenome(const uint32_t size, const FitnessFunc<bool>::fitness fitness_func)
				: PrimitiveGenome<bool>(size, fitness_func) {}

			/**
			   @param index a position

			   Flips the bit at the given position.
			 */
			inline void flip(const uint32_t index)
			{
				(*PrimitiveGenome<bool>::genes)[index].flip();
			}
	};

	/**
	   @}
	 */
}
