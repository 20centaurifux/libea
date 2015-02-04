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
   @file Array.hpp
   @brief foo
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <algorithms.hpp>
#include <cassert>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	template<typename T>
	class Array
	{
		public:
			Array() : _len(0), _size(0), _ptr(nullptr) {}

			virtual ~Array()
			{
				if(_ptr)
				{
					std::free(_ptr);
				}
			}

			void push(T item)
			{
				if(!_size || _len == _size)
				{
					// resize array:
					auto size = _size ? _size * 2 : 16;

					assert(size > _size);
					_size = size;

					if((_ptr = (T*)std::realloc(_ptr, sizeof(T) * _size)) == nullptr)
					{
						throw std::bad_alloc();
					}
				}

				assert(_len < _size);

				_ptr[_len] = item;
				++_len;
			}

			uint32_t size() const
			{
				return _len;
			}

			void clear()
			{
				_len = 0;
			}

			void set(const uint32_t index, T item)
			{
				assert(index < _len);

				_ptr[index] = item;
			}

			T at(const uint32_t index) const
			{
				assert(index < _len);

				return _ptr[index];
			}

		private:
			uint32_t _len;
			uint32_t _size;
			T* _ptr;
	};

	/**
	   @}
	 */
}
#endif
