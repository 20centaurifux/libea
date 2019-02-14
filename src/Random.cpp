/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: sebastian.fedrau@gmail.com
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
 ***************************************************************************/
/**
   @file Random.cpp
   @brief Random number generation utilities.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#include "Random.hpp"

namespace ea::random
{
	RandomEngine default_engine()
	{
		static std::random_device rd;
		std::mt19937 mt(rd());

		return mt;
	}
}

