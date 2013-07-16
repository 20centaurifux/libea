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
   @file InputAdapter.h
   @brief Adapter classes used to read items from wrapped containers.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef IINPUTADAPTER_H
#define IINPUTADAPTER_H

#include <cstdint>
#include <vector>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IInputAdapter
	   @tparam T type of the items stored in the wrapped container
	   @brief Interface for iterator classes.
	 */
	template<class T>
	class IInputAdapter
	{
		public:
			~IInputAdapter() {}

			/**
			   Goes to the next element.
			 */
			virtual void next() = 0;

			/**
			   @return true if iterator has reached last element

			   Checks if the iterator has reached last element.
			 */
			virtual bool end() const = 0; 

			/**
			   Goes to the first element.
			 */
			virtual void first() = 0;

			/**
			   @return container size

			   Gets the size of the wrapped container.
			 */
			virtual uint32_t size() = 0;
		
			/**
			   @return value of the current element

			   Reads the value of the current element.
			 */
			virtual T current() = 0;

			/**
			   @param index location of the item to read
			   @return read value

			   Reads the value at the given position.
			 */
			virtual T at(const uint32_t index) = 0;
	};

	/**
	   @class RandomAccessInputAdapter
	   @tparam T datatype of items stored in the wrapped container
	   @tparam TContainer datatype of the container
	   @brief IInputAdapter wrapping a container. The container has to provide a random_access_iterator.
	 */
	template<typename T, typename TContainer>
	class RandomAccessInputAdapter : public IInputAdapter<T>
	{
		public:
			/**
			   @param begin begin of the range
			   @param end end of the range
			 */
			typedef typename TContainer::iterator Iterator;

			/**
			   @param begin begin of a range
			   @param end end of a range
			 */
			RandomAccessInputAdapter(const Iterator &begin, const Iterator &end) : _begin(begin), _end(end)
			{
				first();
			}

			void next() override
			{
				_iter++;
			}

			bool end() const override
			{
				return _iter == _end;
			}

			void first() override
			{
				_iter = _begin;
			}

			uint32_t size() override
			{
				return _end - _begin;
			}

			T at(const uint32_t index) override
			{
				return *(_begin + index);
			}

			T current() override
			{
				return *_iter;
			}

		private:
			Iterator _iter;
			Iterator _begin;
			Iterator _end;
	};

	/**
	   @param container a container
	   @tparam T datatype of items stored in the given container
	   @tparam TContainer datatype of a container
	   @return a RandomAccessInputAdapter

	   Helper function to create a RandomAccessInputAdapter.
	 */
	template<typename T, typename TContainer>
	RandomAccessInputAdapter<T, TContainer> make_input_adapter(TContainer &container)
	{
		return RandomAccessInputAdapter<T, TContainer>(begin(container), end(container));
	}

	/**
	   @}
	 */
}
#endif
