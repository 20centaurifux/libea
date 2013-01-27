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
 * \file AHash.h
 * \brief Base class for hash algorithms.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef AHASH_H
#define AHASH_H

#include <string>
#include <stdint.h>
#include <assert.h>

namespace ea
{
	#define AHASH_APPEND(from, size)          \
		assert(size <= 8);                \
		std::memcpy(_buffer, from, size); \
		append(_buffer, size);            \
		return *this;

	class AHash
	{
		public:
			~AHash() {}

			AHash& operator<<(const int32_t value)
			{
				AHASH_APPEND(&value, 4);
			}

			AHash& operator<<(const uint32_t value)
			{
				AHASH_APPEND(&value, 4);
			}

			AHash& operator<<(const float value)
			{
				AHASH_APPEND(&value, sizeof(float));
			}

			AHash& operator<<(const double value)
			{
				AHASH_APPEND(&value, sizeof(double));
			}

			AHash& operator<<(const bool value)
			{
				AHASH_APPEND(&value, sizeof(bool));
			}

			AHash& operator<<(const char value)
			{
				_buffer[0] = value;
				append(_buffer, 1);

				return *this;
			}

			AHash& operator<<(const std::string value)
			{
				append(value.c_str(), value.size());

				return *this;
			}

			virtual void reset() = 0;
			virtual void append(const char* buffer, const size_t size) = 0;
			virtual size_t hash() = 0;

		private:
			char _buffer[8];
	};
}
#endif
