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
 * \file Gene.cpp
 * \brief A gene holds a sequence of multiple bits.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <stddef.h>
#include <math.h>
#include <cstring>
#include <stdexcept>
#include <assert.h>
#include "Gene.h"

namespace ea
{
	Gene::Gene(const uint32_t length)
	{
		int count;

		assert(length > 0);

		_length = length;
		count = size();
		_memory = new byte[count];
		memset(_memory, 0, count);
		_hash_set = false;
	}

	Gene::~Gene()
	{
		delete [] _memory;
	}

	bool Gene::operator[] (const uint32_t index)
	{
		return at(index);
	}

	bool Gene::at(const uint32_t index)
	{
		uint32_t offset;
		uint32_t bit;

		Gene::get_offset(index, offset, bit);
		test_offset(index, offset, bit);

		return (_memory[offset] & (1 << bit)) ? true : false;
	}

	void Gene::set(const uint32_t index, const bool value)
	{
		uint32_t offset;
		uint32_t bit;

		Gene::get_offset(index, offset, bit);
		test_offset(index, offset, bit);

		if(value)
		{
			_memory[offset] |= (1 << bit);
		}
		else
		{
			 _memory[offset] &= ~(1 << bit);
		}

		_hash_set = false;
	}

	void Gene::fill(const byte* bytes, const uint32_t offset, const uint32_t count)
	{
		assert(offset >= 0);
		assert(count > 0);

		for(uint32_t i = 0; i < count; ++i)
		{
			_memory[i] = bytes[offset + i];
		}

		_hash_set = false;
	}

	void Gene::read(const uint32_t offset, byte* memory, const uint32_t count) const
	{
		if(offset >= GENE_ARRAY_SIZE(length() - count))
		{
			throw std::out_of_range("index is out of range");
		}

		memcpy(memory, _memory + offset, count);
	}

	byte Gene::read_byte(const uint32_t offset) const
	{
		if(offset >= GENE_ARRAY_SIZE(length()))
		{
			throw std::out_of_range("index is out of range");
		}

		return _memory[offset];
	}

	int32_t Gene::read_int32(const uint32_t offset) const
	{
		int32_t number;

		if(offset >= GENE_ARRAY_SIZE(length() - 4))
		{
			throw std::out_of_range("index is out of range");
		}

		memcpy(&number, _memory + offset, 4);

		return number;
	}

	void Gene::clear()
	{
		memset(_memory, 0, GENE_ARRAY_SIZE(_length));
		_hash_set = false;
	}

	bool Gene::equals(const Gene* gene)
	{
		int count = GENE_ARRAY_SIZE(length());

		if(!gene)
		{
			return false;
		}

		if(gene->length() != _length)
		{
			return false;
		}

		for(int i = 0; i < count; ++i)
		{
			if(_memory[i] != gene->_memory[i])
			{
				return false;
			}
		}

		return true;
	}

	Gene* Gene::clone()
	{
		Gene *gene = new Gene(_length);

		gene->fill(_memory, 0, GENE_ARRAY_SIZE(_length));

		return gene;
	}

	size_t Gene::hash() const
	{
		if(_hash_set)
		{
			return _hash;
		}

		_hash = 0;

		for(uint32_t i = 0; i < size(); ++i)
		{
			_hash = _memory[i] + (_hash << 6) + (_hash << 16) - _hash;
		}

		return _hash;
	}

	void Gene::get_offset(const uint32_t index, uint32_t& offset, uint32_t& bit)
	{
		offset = index / 8;
		bit = index - (offset * 8);
	}

	void Gene::test_offset(const uint32_t index, const uint32_t offset, const uint32_t bit)
	{
		if(index > 0)
		{
			if(index < length())
			{
				return;
			}
		}

		throw std::out_of_range("index is out of range");
	}
}
