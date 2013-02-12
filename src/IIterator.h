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
   @brief Interface for iterator classes
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef IITERATOR_H
#define IITERATOR_H

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IIterator
	   @tparam datatype of items stored in container
	   @brief Interface for iterator classes
	 */
	template<class T>
	class IIterator
	{
		public:
			/**
			   Goes to next element.
			 */
			virtual void next() = 0;

			/**
			   @return true of iterator has reached last element

			   Checks if iterator has reached last element.
			 */
			virtual bool end() const = 0; 

			/**
			   Goes to first element.
			 */
			virtual void first() = 0;

			/**
			   @return range size

			   Gets size of the range.
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
	   @}
	 */
}
#endif
