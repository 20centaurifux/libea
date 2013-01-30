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
   @file ICloneable.h
   @brief Interface for cloneable classes.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ICLONEABLE_H
#define ICLONEABLE_H

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class ICloneable
	   @tparam T Datatype of instances the clone method create.
	   @brief Interface for classes supporting cloning. The clone method creates a new instance of a
	          class with the same value as an existing instance.
	 */
	template<class T>
	class ICloneable
	{
		public:
			virtual ~ICloneable() {};

			/**
			   @return a duplicate instance of a class

			   Clones a class.
			 */
			virtual T* clone() const = 0;
	};

	/**
	   @}
	 */
}
#endif
