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
   @file IInserter.h
   @brief Interface for wrapper classes used to insert data.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef IINSERTER_H
#define IINSERTER_H

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IInserter
	   @tparam datatype of items stored in container
	   @brief Interface for wrapper classes used to insert data.
	 */
	template<class T>
	class IInserter
	{
		public:
			virtual ~IInserter() {};

			/**
			   @param value value to insert

			   Inserts a value into a container.
			 */
			virtual void insert(T value) = 0;
	};

	/**
	   @}
	 */
}
#endif

