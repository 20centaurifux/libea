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
 * \brief A genome holds multiple genes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <stdexcept>
#include <cassert>
#include "Genome.h"
#include "foreach.h"

using namespace std;

namespace ea
{
	Genome::Genome()
	{
		_genes = new vector<Gene*>();
	}

	Genome::Genome(const uint32_t size)
	{
		_genes = new vector<Gene*>(size);
	}

	Genome::~Genome()
	{
		foreach<Gene*>(*_genes, delete_object<Gene>);
		delete _genes;
	}

	Gene* Genome::append_gene(Gene* gene)
	{
		assert(gene != NULL);

		_genes->push_back(gene);

		return gene;
	}

	Gene* Genome::append_new_gene(const int32_t length)
	{
		Gene *gene;

		assert(gene != NULL);

		gene = new Gene(length);
		_genes->push_back(gene);

		return gene;
	}

	void Genome::set_gene(const uint32_t index, Gene* gene)
	{
		if(index >= _genes->size())
		{
			throw std::out_of_range("index is out of range");
		}

		(*_genes)[index] = gene;
	}

	Gene& Genome::operator[] (const uint32_t index)
	{
		return (*gene_at(index));
	}

	Gene* Genome::gene_at(const uint32_t index) const
	{
		if(index >= _genes->size())
		{
			throw std::out_of_range("index is out of range");
		}

		return (*_genes)[index];
	}

	void Genome::remove_gene(const uint32_t index)
	{
		_genes->erase(_genes->begin() + index);
	}

	int32_t Genome::find_gene(const Gene* gene) const
	{
		for(int32_t i = 0; i < (int32_t)_genes->size(); i++)
		{
			if((*_genes)[i]->equals(gene))
			{
				return i;
			}
		}

		return -1;
	}
}
