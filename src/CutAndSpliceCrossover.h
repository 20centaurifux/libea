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
 * \file CutAndSpliceCrossover.h
 * \brief Implementation of cut and splice crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef CUTANDSPLICECROSSOVER_H
#define CUTANDSPLICECROSSOVER_H

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	class CutAndSpliceCrossover : public ACrossover
	{
		public:
			CutAndSpliceCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover(rnd_generator) {}
			virtual ~CutAndSpliceCrossover() {};
			uint32_t crossover(const ea::Individual* a, const ea::Individual* b, std::vector<Individual*>& children);
	};
}
#endif
