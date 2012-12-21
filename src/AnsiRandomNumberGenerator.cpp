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
 * \file AnsiRandomNumberGenerator.cpp
 * \brief ANSI compatible random number generator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <ctime>
#include "AnsiRandomNumberGenerator.h"

using namespace std;

namespace ea
{
	AnsiRandomNumberGenerator::AnsiRandomNumberGenerator()
	{
		srand(time(NULL));
	}

	int32_t AnsiRandomNumberGenerator::random()
	{
		return rand();
	}

	int32_t AnsiRandomNumberGenerator::get_max() const
	{
		return RAND_MAX;
	}
}
