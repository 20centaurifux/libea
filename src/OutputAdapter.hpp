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
   @file OutputAdapter.hpp
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
	   @class STLBackInserterAdapter
	   @tparam TContainer a STL container
	   @brief An IOutputAdapter creating a std::back_insert_iterator from a container.
	 */
	template<typename TContainer>
	class STLBackInserterAdapter : public IOutputAdapter<typename TContainer::value_type>
	{
		public:
			/**
			   @param container a STL container
			 */
			STLBackInserterAdapter(TContainer& container) : _iterator(std::back_inserter(container)) {}

			void push(typename TContainer::value_type item) override
			{
				*_iterator++ = item;
			}

		private:
			std::back_insert_iterator<TContainer> _iterator;
	};

	/**
	   @param container a container
	   @tparam TContainer datatype of a container
	   @return an OutputAdapter

	   Helper function to create an OutputAdapter.
	 */
	template<typename TContainer>
	STLBackInserterAdapter<TContainer> make_output_adapter(TContainer &container)
	{
		return STLBackInserterAdapter<TContainer>(container);
	}

	/**
	   @}
	 */
}
#endif
