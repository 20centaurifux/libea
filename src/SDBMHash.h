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
   @file SDBMHash.h
   @brief SDBM hash algorithm.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */

#ifndef SDBMHASH_H
#define SDBMHASH_H

#include "AHash.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class SDBMHash
	   @brief Implementation of the SDBM hash algorithm.
	 */
	class SDBMHash : public AHash
	{
		public:
			SDBMHash() : _hash(0) {}

			inline void reset()
			{
				_hash = 0;
			}

			inline void append(const char* buffer, const size_t size)
			{
				for(uint32_t i = 0; i < size; i++)
				{
					_hash = buffer[i] + (_hash << 6) + (_hash << 16) - _hash;
				}
			}

			inline size_t hash()
			{
			
				return _hash;
			}

		private:
			size_t _hash;
	};

	/**
	   @}
	 */
}
#endif