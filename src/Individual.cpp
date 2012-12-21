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
 * \file Individual.cpp
 * \brief An individual holds a genome and a fitness function.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <stddef.h>
#include <assert.h>
#include "Individual.h"

using namespace std;

namespace ea
{
	Individual::Individual(const FitnessFunc fitness) : Genome()
	{
		init(fitness);
	}
	
	Individual::Individual(const FitnessFunc fitness, uint32_t size) : Genome(size)
	{
		init(fitness);
	}

	float Individual::fitness()
	{
		if(!_fitness_set)
		{
			_fitness = _fitness_func(this);
			_fitness_set = true;
		}

		return _fitness;
	}

	void Individual::init(const FitnessFunc fitness)
	{
		assert(fitness != NULL);

		_fitness_func = fitness;
		_fitness_set = false;
		_fitness = 0;
	}
}
