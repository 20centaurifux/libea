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
/*!
 * \file SingleSwapMutation.h
 * \brief Mutation operator to swap 2 genes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef SINGLESWAPMUTATION_H
#define SINGLESWAPMUTATION_H

#include "AMutation.h"

namespace ea
{
	template<class T>
	class SingleSwapMutation : public AMutation<T>
	{
		public:
			SingleSwapMutation(ARandomNumberGenerator* rnd_generator) : AMutation<T>(rnd_generator) {}
			~SingleSwapMutation() {};

			void mutate(AGenome<T>* genome)
			{
				int32_t offsets[2];

				if(genome->size() <= 1)
				{
					return;
				}

				AMutation<T>::generator->get_unique_numbers(0, genome->size() - 1, offsets, 2);
				genome->swap(offsets[0], offsets[1]);
			}
	};
}
#endif
