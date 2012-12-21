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
 * \file OnePointCrossover.cpp
 * \brief Implementation of one-point crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef ONEPOINTCROSSOVER_H
#define ONEPOINTCROSSOVER_H

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	class OnePointCrossover : public ACrossover
	{
		public:
			OnePointCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover(rnd_generator) {}
			virtual ~OnePointCrossover() {};
			uint32_t crossover(const ea::Individual* a, const ea::Individual* b, std::vector<Individual*>& children);

		private:
			Individual* crossover(const Individual* a, const Individual* b, const uint32_t separator) const;
	};
}
#endif