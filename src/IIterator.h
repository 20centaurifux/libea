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
	   @brief Interface for iterator classes
	 */
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
			   Go to first element.
			 */
			virtual void first() = 0;

			/**
			   @return range size

			   Gets size of the range.
			 */
			virtual uint32_t size() = 0;

			/**
			   @tparam T type of the value to read
			   @param value location to store read value

			   Reads the value of the current element.
			 */
			template<class T>
			void current(T& value)
			{
				value = reinterpret_cast<T>(get_current());
			}

			/**
			   @tparam T type of the value to read
			   @param index location of the item to read
			   @param value location to store read value

			   Reads the value at the given position.
			 */
			template<class T>
			void at(const uint32_t index, T& value)
			{
				value = reinterpret_cast<T>(get_item(index));
			}

		private:
			/**
			   @return value of the current element

			   Reads the value of the current element.
			 */
			virtual void* get_current() = 0;

			/**
			   @param index location of the item to read
			   @return read value

			   Reads the value at the given position.
			 */
			virtual void* get_item(const uint32_t index) = 0;
	};

	/**
	   @}
	 */
}
#endif
