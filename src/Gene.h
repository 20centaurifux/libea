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
 * \file Gene.h
 * \brief A gene holds a sequence of multiple bits.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef GENE_H
#define GENE_H

#include <math.h>
#include "ISerializable.h"
#include "IEquatable.h"
#include "ICloneable.h"
#include "IHashfunction.h"


#define GENE_ARRAY_SIZE(bits) (uint32_t)ceil((float)bits / 8)

namespace ea
{
	class Gene : public ISerializable, public IEquatable<Gene>, public ICloneable<Gene>, public IHashfunction
	{
		public:
			Gene(const uint32_t length);
			virtual ~Gene();
			inline uint32_t length() const { return _length; }
			inline uint32_t size() const { return GENE_ARRAY_SIZE(_length); }
			bool operator[] (const uint32_t index);
			bool at(const uint32_t index);
			void set(const uint32_t index, const bool value);
			void fill(const byte* bytes, const uint32_t offset, const uint32_t count);
			void read(const uint32_t offset, byte* memory, const uint32_t count) const;
			byte read_byte(const uint32_t offset) const;
			int32_t read_int32(const uint32_t offset) const;
			inline uint32_t read_uint32(const uint32_t offset) const { return (uint32_t)read_int32(offset); }
			void clear();
			bool equals(const Gene* gene);
			virtual Gene* clone();
			size_t hash() const;

		private:
			uint32_t _length;
			byte *_memory;
			mutable size_t _hash;
			mutable bool _hash_set;

			static void get_offset(const uint32_t index, uint32_t& offset, uint32_t &bit);
			void test_offset(const uint32_t index, const uint32_t offset, const uint32_t bit);
	};
}
#endif
