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
   @file IEquatable.h
   @brief Interface for classes which can be checked for equality.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef IEQUATABLE_H
#define IEQUATABLE_H

#include <vector>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class IEquatable
	   @tparam T Type of the object to compare.
	   @brief Supports a method for determining equality of instances.
	 */
	template<class T>
	class IEquatable
	{
		public:
			/**
			   @param object instance to check for equality
			   @return true if instances are equal

			   Determines equality of two instance.
			  */
			virtual bool equals(const T* object) const = 0;

		protected:
			virtual ~IEquatable() {}
	};

	/**
	   @}
	 */
}
#endif
