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
 * \file IIndividual.h
 * \brief Interface for individual.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef IINDIVIDUAL_H
#define IINDIVIDUAL_H

#include <stdint.h>
#include "AGenome.h"

namespace ea
{
	template<class T>
	class IIndividual : public AGenome<T>
	{
		public:
			IIndividual(uint32_t size) : AGenome<T>(size) {}
			virtual ~IIndividual() {}
			virtual float fitness();
	};
}
#endif
