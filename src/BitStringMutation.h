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
   @brief Inverts all bits of a genome.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
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
	   @brief Inverts all bits of the genome.
	 */
	class BitStringMutation : public ABinaryMutation
	{
		public:
			using ABinaryMutation::mutate;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			BitStringMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ABinaryMutation(rnd_generator) {}

			/**
			   @param genome genome to mutate
			 
			   Mutates a genome.
			 */
			void mutate(std::shared_ptr<PrimitiveGenome<bool>> genome) override
			{
				bool flipped = false;

				while(!flipped)
				{
					for(uint32_t i = 0; i < genome->size(); ++i)
					{
						if(!this->generator->get_int32(0, genome->size() - 1))
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
