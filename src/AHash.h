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
   @file AHash.h
   @brief Base class for hash algorithms.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */

#ifndef AHASH_H
#define AHASH_H

#include <string>
#include <cstring>
#include <stdint.h>
#include <assert.h>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/*! Helper to copy a generic to the buffer. */
	#define AHASH_APPEND(from, size)          \
		assert(size <= 8);                \
		memcpy(_buffer, from, size); \
		append(_buffer, size);            \
		return *this;

	/**
	   @class AHash
	   @brief Abstract base class for hash algorithms.
	 */
	class AHash
	{
		public:
			~AHash() {}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends int32_t value to buffer.
			 */
			AHash& operator<<(const int32_t value)
			{
				AHASH_APPEND(&value, 4);
			}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends uint32_t value to buffer.
			 */
			AHash& operator<<(const uint32_t value)
			{
				AHASH_APPEND(&value, 4);
			}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends float value to buffer.
			 */
			AHash& operator<<(const float value)
			{
				AHASH_APPEND(&value, sizeof(float));
			}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends double value to buffer.
			 */
			AHash& operator<<(const double value)
			{
				AHASH_APPEND(&value, sizeof(double));
			}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends double value to buffer.
			 */
			AHash& operator<<(const size_t value)
			{
				AHASH_APPEND(&value, sizeof(size_t));
			}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends bool value to buffer.
			 */
			AHash& operator<<(const bool value)
			{
				AHASH_APPEND(&value, sizeof(bool));
			}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends char value to buffer.
			 */
			AHash& operator<<(const char value)
			{
				_buffer[0] = value;
				append(_buffer, 1);

				return *this;
			}

			/**
			   @param value value to append
			   @return reference to the hash algorithm
			   
			   Appends string value to buffer.
			 */
			AHash& operator<<(const std::string value)
			{
				append(value.c_str(), value.size());

				return *this;
			}

			/**
			   Resets the buffer.
			 */
			virtual void reset() = 0;

			/**
			   @param buffer char buffer to append
			   @param size size of the char buffer

			   Appends char buffer to buffer.
			 */
			virtual void append(const char* buffer, const size_t size) = 0;

			/**
			   @return hash hash value

			   Calculates the hash value of the buffer.
			 */
			virtual size_t hash() = 0;

		private:
			char _buffer[8];
	};

	/**
	   @}
	 */
}
#endif
