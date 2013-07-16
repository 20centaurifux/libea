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
   @file join.h
   @brief A function to join strings.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef JOIN_H
#define JOIN_H

#include <string>
#include <sstream>
#include <algorithm>

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
	   @tparam T type of the items to join
	   @tparam TIterator type of the specified iterators
	   @return a string

	   Joins elements of a container to a string.
	 */
	template<class T, class TIterator>
	std::string join(const TIterator &begin, const TIterator &end, const std::string &separator)
	{
		std::ostringstream stream;

		stream << *begin;

		std::for_each(begin + 1, end, [&stream, &separator] (const T& value) { stream << separator << value; });

		return stream.str();
	}

	/**
	   @}
	 */
}
#endif
