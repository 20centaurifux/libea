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
 * \file ACrossover.h
 * \brief Crossover operator base class.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef ACROSSOVER_H
#define ACROSSOVER_H

#include <vector>
#include <cassert>
#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	template<class T>
	class ACrossover
	{
		public:
			ACrossover(ARandomNumberGenerator* rnd_generator) : generator(rnd_generator) {}
			virtual ~ACrossover() {};
			virtual uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, std::vector<AGenome<T>*>& children) = 0;

		protected:
			ARandomNumberGenerator* generator;
	};
}
#endif
