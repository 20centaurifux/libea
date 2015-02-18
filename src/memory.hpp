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
   @file memory.hpp
   @brief Memory functions.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */
#ifndef MEMORY_H
#define MEMORY_H

#include <cstdlib>
#include <memory>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IAllocator
	   @brief Interface for basic memory functions.
	  */
	class IAllocator
	{
		public:
			/**
			   @param size number of bytes to allocate
			   @return pointer to a block of memory

			   Allocates memory.
			 */
			virtual void* alloc(const size_t size) = 0;

			/**
			   @param ptr memory to free

			   Frees memory.
			 */
			virtual void free(void* ptr) = 0;
	};

	/**
	   @class StdAllocator
	   @brief A wrapper for std::malloc() and std::free().
	 */
	class StdAllocator : public ea::IAllocator
	{
		public:
			void* alloc(const size_t size) override
			{
				void* ptr = std::malloc(size);

				if(ptr == nullptr)
				{
					throw std::bad_alloc();
				}

				return ptr;
			}

			void free(void *ptr) override
			{
				std::free(ptr);
			}

			/**
			   @return Shared pointer to a new StdAllocator

			   Creates a shared pointer to a new StdAllocator.
			  */
			static std::shared_ptr<StdAllocator> create_shared()
			{
				return std::make_shared<StdAllocator>();
			}
	};

	/**
	   @}
	 */
}
#endif
