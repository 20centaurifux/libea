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
 * \file ACrossover.cpp
 * \brief Crossover operator base class.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include "ACrossover.h"

using namespace std;

namespace ea
{
	uint32_t ACrossover::crossover(const std::vector<Individual*> a, const std::vector<Individual*> b, std::vector<Individual*>& children)
	{
		uint32_t sum = 0;

		for(uint32_t i = 0; i < a.size(); i++)
		{
			for(uint32_t j = 0; j < b.size(); j++)
			{
				sum += crossover(a[i], b[j], children);
			}
		}

		return sum;
	}
}
