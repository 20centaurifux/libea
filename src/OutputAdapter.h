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
   @brief Adapter classes used to add items to wrapped containers.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef OUTPUT_ADAPTER_H
#define OUTPUT_ADAPTER_H

#include <vector>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IOutputAdapter
	   @tparam T datatype of items stored in the wrapped container
	   @brief Interface for adapter classes used to append data to a container.
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
			virtual void append(T item) = 0;
	};

	/**
	   @class BackInsertOutputAdapter
	   @tparam T datatype of items stored in the wrapped container
	   @tparam TContainer datatype of the wrapped container
	   @brief An IOutputAdapter wrapping a container. The container needs to provide a push_back() function.
	 */
	template<typename T, typename TContainer>
	class BackInsertOutputAdapter : public IOutputAdapter<T>
	{
		private:
			TContainer &_container;

		public:
			/**
			   @param container a container to wrap
			 */
			BackInsertOutputAdapter(TContainer &container) : _container(container) {}

			void append(T item) override
			{
				_container.push_back(item);
			}
	};

	/**
	   @param container a container
	   @tparam T datatype of items stored in the specified container
	   @tparam TContainer datatype of a container
	   @return a BackInsertOutputAdapter

	   Helper function to create BackInsertOutputAdapters.
	 */
	template<typename T, typename TContainer>
	BackInsertOutputAdapter<T, TContainer> make_output_adapter(TContainer &container)
	{
		return BackInsertOutputAdapter<T, TContainer>(container);
	}

	/**
	   @}
	 */
}
#endif