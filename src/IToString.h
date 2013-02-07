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
   @file IToString.h
   @brief Interface for classes which can be converted to a string.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ITOSTRING_H
#define ITOSTRING_H

#include <string>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IToString
	   @brief Interface for classes which can be converted to a string.
	 */
	class IToString
	{
		public:
			/**
			   @return a string

			   Returning a string representing the object.
			 */
			virtual std::string to_string() = 0;
	};

	/**
	   @}
	 */
}
#endif

