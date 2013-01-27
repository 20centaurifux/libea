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
 * \file ARandomNumberGenerator.h
 * \brief Base class for random number generators.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef IRANDOMNUMBERGENERATOR_H
#define IRANDOMNUMBERGENERATOR_H

#include <stdint.h>
#include <stdlib.h>

namespace ea
{
	class ARandomNumberGenerator
	{
		public:
			ARandomNumberGenerator() {}
			virtual ~ARandomNumberGenerator() {}
			virtual int32_t get_number(const int32_t min, const int32_t max);
			virtual void get_numbers(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length);
			virtual void get_unique_numbers(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length);
			virtual int32_t random() = 0;
			virtual int32_t get_max() const = 0;
	};
}
#endif
