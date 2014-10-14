/***************************************************************************
    begin........: May 2013
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
   @file OutputAdapter.h
   @brief Adapters used to write items to wrapped containers.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef OUTPUT_ADAPTER_H
#define OUTPUT_ADAPTER_H

#include <vector>
#include <iterator>
#include <memory>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IOutputAdapter
	   @tparam T datatype of items stored in the wrapped container
	   @brief Interface for adapter classes used to write items into a container.
	 */
	template<typename T>
	class IOutputAdapter
	{
		public:
			virtual ~IOutputAdapter() {};

			/**
			   @param item item to insert

			   Inserts a value into a container.
			 */
			virtual void push(T item) = 0;
	};

	/**
	   @class STLOutputAdapter
	   @tparam T datatype of items stored in the wrapped container
	   @tparam TIterator an iterator compatible to STL's output iterator
	   @brief An IOutputAdapter wrapping an iterator compatible to STL's output iterator.
	 */
	template<typename T, typename TIterator>
	class STLOutputAdapter : public IOutputAdapter<T>
	{
		private:
			TIterator& _iterator;

		public:
			/**
			   @param iterator a STL iterator to wrap
			 */
			STLOutputAdapter(TIterator& iterator) : _iterator(iterator) {}

			void push(T item) override
			{
				*_iterator++ = item;
			}
	};

	/**
	   \typedef STLVectorAdapter
	   \tparam TSequence type of the sequence stored in a std::vector
	   \brief A STLOutputAdapter wrapping a std::back_insert_iterator created from a std::vector.
	 */
	template<typename TSequence>
	using STLVectorAdapter = STLOutputAdapter<TSequence, std::back_insert_iterator<std::vector<TSequence>>>;

	/**
	   @}
	 */
}
#endif
