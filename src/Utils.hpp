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

	/*! @} */
}

#endif

