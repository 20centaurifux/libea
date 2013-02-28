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
   @file DoubleSwapMutation.h
   @brief Mutation operator to swap 3 genes.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef DOUBLESWAPMUTATION_H
#define DOUBLESWAPMUTATION_H

#include <cstring>

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
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Swaps three genes.
	 */
	template<class T>
	class DoubleSwapMutation : public AMutation<T>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			DoubleSwapMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : AMutation<T>(rnd_generator) {}

			~DoubleSwapMutation() {};

			void mutate(AGenome<T>* genome)
			{
				int32_t offsets[3];

				if(genome->size() <= 2)
				{
					return;
				}

				AMutation<T>::generator->get_unique_numbers(0, genome->size() - 1, offsets, 3);

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
