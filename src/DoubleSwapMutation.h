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
 * \file DoubleSwapMutation.h
 * \brief Mutation operator to swap 2 genes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef DOUBLESWAPMUTATION_H
#define DOUBLESWAPMUTATION_H

#include "AMutation.h"
#include "ARandomNumberGenerator.h"
#include <cstring>

namespace ea
{
	class DoubleSwapMutation : public AMutation
	{
		public:
			DoubleSwapMutation(ARandomNumberGenerator* rnd_generator) : AMutation(rnd_generator) {}
			~DoubleSwapMutation() {};

			void mutate(ea::Genome* genome)
			{
				int32_t offsets[3];

				if(genome->size() <= 2)
				{
					return;
				}

				generator->get_unique_numbers(0, genome->size() - 1, offsets, 3);

				genome->swap(offsets[0],offsets[1], offsets[2]);
			}
	};
}
#endif
