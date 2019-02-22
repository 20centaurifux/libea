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
   @file Utils.hpp
   @brief Miscellaneous utility functions.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_UTILS_HPP
#define EA_UTILS_HPP

#include <iterator>

namespace ea::utils
{
	/**
	   @addtogroup Utils
	   @{
	 */

	/**
	   @tparam F a function object: f()
	   @param count number of repeats
	   @param fn function to repeat

	   Calls \p fn \p count times.
	 */
	template<typename F>
	void repeat(const size_t count, F fn)
	{
		for(size_t i = 0; i < count; ++i)
		{
			fn();
		}
	}

	/**
	   @tparam InputIterator must meet the requirements of LegacyForwardIterator
	   @param it an iterator 
	   @param first iterator pointing to the first element of a sequence
	   @param last iterator pointing to the end (element after the last element) of a sequence
	   @return an iterator

	   Returns the successor of \p it. If \p it is the last element of the sequence \p first
	   is returned.
	 */
	template<typename InputIterator>
	InputIterator next(InputIterator it, InputIterator first, InputIterator last)
	{
		InputIterator iterator = std::next(it);

		if(iterator == last)
		{
			iterator = first;
		}

		return iterator;
	}

	/**
	   @tparam InputIterator must meet the requirements of LegacyBidirectionalIterator
	   @param it an iterator 
	   @param first iterator pointing to the first element of a sequence
	   @param last iterator pointing to the end (element after the last element) of a sequence
	   @return an iterator

	   Returns the predecessor of \p it. If \p it has no predecessor the last element of the
	   sequence is returned.
	 */
	template<typename InputIterator>
	InputIterator prev(InputIterator it, InputIterator first, InputIterator last)
	{
		return std::prev(it == first ? last : it);
	}

	/*! @} */
}

#endif

