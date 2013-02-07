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
   @file join.h
   @brief A function to join strings.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef JOIN_H
#define JOIN_H

#include <string>
#include <sstream>
#include <algorithm>

#include "IToString.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @param begin first element of a container
	   @param end last element of a container
	   @param separator a separator
	   @return a string

	   Joins elements of a container to a string.
	 */
	template<class T, class A>
	std::string join(const A& begin, const A& end, const std::string& separator)
	{
		std::ostringstream stream;

		stream << *begin;

		std::for_each(begin + 1, end, [&stream, &separator] (const T& value) { stream << separator << value; });

		return stream.str();
	}

	/**
	   @param begin first element of a container
	   @param end last element of a container
	   @return a string

	   Joins elements of a container to a string.
	 */
	template<class T, class A>
	std::string join(const A& begin, const A& end)
	{
		std::ostringstream stream;

		stream << *begin;

		std::for_each(begin + 1, end, [&stream] (const T& value) { stream << value; });

		return stream.str();
	}

	/**
	   @}
	 */
}
#endif
