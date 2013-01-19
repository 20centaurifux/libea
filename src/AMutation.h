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
 * \file AMutation.h
 * \brief Base class for mutation operators.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef AMUTATION_H
#define AMUTATION_H

#include "Genome.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	class AMutation
	{
		public:
			AMutation(ARandomNumberGenerator* rnd_generator) { generator = rnd_generator; }
			virtual ~AMutation() {};
			virtual void mutate(Genome* genome) = 0;

		protected:
			ARandomNumberGenerator* generator;
	};
}
#endif
