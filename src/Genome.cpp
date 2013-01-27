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
 * \file Genome.cpp
 * \brief A genome holds objects inherited from the AGene base class.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <algorithm>
#include "Genome.h"

using namespace std;

namespace ea
{
	float Genome::fitness()
	{
		if(_modified)
		{
			_fitness =  fitness_func(*this);
			_modified = false;
		}

		return _fitness;
	}

	void Genome::set(const uint32_t index, AGene* gene)
	{
		if((*_genes)[index])
		{
			delete ((*_genes)[index]);
		}

		(*_genes)[index] = gene;

		gene->attach_listener(this);
	}

	void Genome::copy_to(const uint32_t index, AGene* gene)
	{
		AGene* clone = gene->clone();

		set(index, clone);

		clone->attach_listener(this);
	}

	AGene* Genome::at(const uint32_t index) const
	{
		return (*_genes)[index];
	}

	Genome* Genome::instance(const uint32_t size) const
	{
		return new Genome(size, fitness_func);
	}

	bool Genome::find(AGene* gene, uint32_t& index) const
	{
		auto iter = std::find(_genes->begin(), _genes->end(), gene);

		if(iter != _genes->end())
		{
			index = iter - _genes->begin();
			return true;
		}

		return false;
	}

	bool Genome::contains(AGene* gene) const
	{
		return !(std::find(_genes->begin(), _genes->end(), gene) == _genes->end());
	}

	void Genome::swap(const uint32_t pos1, const uint32_t pos2) const
	{
		AGene* temp;

		temp = (*_genes)[pos1];
		(*_genes)[pos1] = (*_genes)[pos2];
		(*_genes)[pos2] = temp;
	}
}
