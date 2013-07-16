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
   @file Observable.h
   @brief Base class for observable classes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include <algorithm>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class Observable
	   @tparam T datatype of related listener class
	   @brief Base class for observable objects. Can hold multiple listener classes.
	 */
	template<typename T>
	class Observable
	{
		public:
			virtual ~Observable() {}

			/**
			   @tparam F function called for each assigned listener

			   Call a callback function for each assigned listener.
			 */
			template<typename F>
			F for_each(F f)
			{
				std::for_each(begin(_listener), end(_listener), f);

				return f;
			}

			/**
			   @param l a listener class

			   Assigns a listener to the observable.
			 */
			void attach_listener(const T l)
			{
				_listener.push_back(l);
			}


			/**
			   @param l a listener class

			   Detachs a listener from the observable.
			 */
			void detach_listener(const T l)
			{
				auto pos = std::find(begin(_listener), end(_listener), l);

				if(pos != _listener.end())
				{
					_listener.erase(pos);
				}
			}
		
		private:
			std::vector<T> _listener;
	};

	/**
	   @}
	 */
}
#endif
