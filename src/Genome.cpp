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
#include <algorithm>
#include "Genome.h"
#include "foreach.h"

using namespace std;

namespace ea
{
	Genome::Genome(const uint32_t size) : AGenome(size)
	{
		_genes = new vector<Gene*>(size);
		_hash_set = false;
	}

	Genome::~Genome()
	{
		foreach<Gene*>(*_genes, delete_object<Gene>);
		delete _genes;
	}

	void Genome::set(const uint32_t index, Gene* gene)
	{
		if(index >= _genes->size())
		{
			throw std::out_of_range("index is out of range");
		}

		if((*_genes)[index])
		{
			delete (*_genes)[index];
		}

		(*_genes)[index] = gene;
		_hash_set = false;
		invoke_listener(GENOME_EVENT_SET, index);
		gene->attach_listener(this);
	}

	void Genome::copy_to(const uint32_t index, Gene* gene)
	{
		set(index, gene->clone());
	}

	Gene* Genome::at(const uint32_t index) const
	{
		if(index >= _genes->size())
		{
			throw out_of_range("index is out of range");
		}

		return (*_genes)[index];
	}

	bool Genome::find(Gene* gene, uint32_t& index) const
	{
		for(int32_t i = 0; i < (int32_t)_genes->size(); i++)
		{
			if((*_genes)[i] && (*_genes)[i]->equals(gene))
			{
				index = i;
				return true;
			}
		}

		return false;
	}

	bool Genome::contains(Gene* gene) const
	{
		for(int32_t i = 0; i < (int32_t)_genes->size(); i++)
		{
			if((*_genes)[i] && (*_genes)[i]->equals(gene))
			{
				return true;
			}
		}

		return false;
	}

	void Genome::swap(const uint32_t pos1, const uint32_t pos2) const
	{
		Gene* gene;

		if(pos1 >= _genes->size() || pos2 >= _genes->size())
		{
			throw std::out_of_range("index out of range");
		}

		gene = (*_genes)[pos1];

		(*_genes)[pos1] = (*_genes)[pos2];
		(*_genes)[pos2] = gene;
	}

	size_t Genome::hash() const
	{
		byte* buffer = NULL;
		uint32_t buffer_size = 0;
		uint32_t i;

		if(_hash_set)
		{
			return _hash;
		}

		_hash = 0;
		_hash_set = true;

		// read memory of each individual:
		for(vector<Gene*>::iterator it = _genes->begin() ; it != _genes->end(); ++it)
		{
			if((i = (*it)->size()) > buffer_size)
			{
				buffer_size = i;
				buffer = (byte*)realloc(buffer, buffer_size);
			}

			(*it)->read(0, buffer, buffer_size);

			// update hash:
			for(i = 0; i < buffer_size; ++i)
			{
				_hash = buffer[i] + (_hash << 6) + (_hash << 16) - _hash;
			}
		}

		free(buffer);

		return _hash;
	}

	void Genome::modified(const Gene* sender, const GeneEventArg* arg)
	{
		for(vector<GenomeListener*>::iterator it = listener.begin(); it != listener.end(); ++it)
		{
			(*it)->gene_set(this, arg);
		}
	}

	void Genome::cleared(const Gene* sender)
	{
		for(vector<GenomeListener*>::iterator it = listener.begin(); it != listener.end(); ++it)
		{
			(*it)->gene_cleared(this);
		}
	}

	void Genome::invoke_listener(GENOME_EVENT event, const uint32_t offset)
	{
		GenomeEventArg arg;

		for(vector<GenomeListener*>::iterator it = listener.begin(); it != listener.end(); ++it)
		{
			arg.offset = offset;
			arg.gene = at(offset);

			if(event == GENOME_EVENT_SET)
			{
				(*it)->modified(this, &arg);
			}
			else if(event == GENOME_EVENT_ADDED)
			{
				(*it)->created(this, &arg);
			}
			else
			{
				(*it)->deleted(this, &arg);
			}
		}
	}
}
