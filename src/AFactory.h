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
 * \file AFactory.h
 * \brief Factories are used to create (random) instances of individuals.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef AFACTORY_H
#define AFACTORY_H

#include<vector>
#include "ARandomNumberGenerator.h"

namespace ea
{
	template<class T>
	class AFactory
	{
		public:
			AFactory(ARandomNumberGenerator* rnd_generator) : generator(rnd_generator) {}
			virtual ~AFactory() {}
			virtual std::vector<T> random(const uint32_t count) = 0;

		protected:
			ARandomNumberGenerator* generator;
	};
}
#endif
