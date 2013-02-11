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
/**
   @file IIterator.h
   @brief Adapter class for STL iterators.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ITERATORADAPTER_H
#define ITERATORADAPTER_H

#include "IIterator.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IteratorAdapter
	   @tparam Iterator type of the STL iterator
	   @tparam T type of the value stored in the related container
	   @brief Adapter class for STL iterators.
	 */
	template<class Iterator, class T>
	class IteratorAdapter : public IIterator
	{
		public:
			/**
			   @param begin begin of the range
			   @param end end of the range
			 */
			IteratorAdapter(const Iterator& begin, const Iterator& end) : _begin(begin), _end(end)
			{
				first();
			}

			void next()
			{
				++_iter;
			}

			bool end() const
			{
				return _iter == _end;
			}

			void first()
			{
				_iter = _begin;
			}

			uint32_t size()
			{
				return _end - _begin;
			}

		private:
			Iterator _iter;
			Iterator _begin;
			Iterator _end;

			void* get_current()
			{
				return reinterpret_cast<void*>(*_iter);
			}

			void* get_item(const uint32_t index)
			{
				return reinterpret_cast<void*>(*(_iter + index));
			}
	};

	/**
	   @}
	 */
}
#endif
