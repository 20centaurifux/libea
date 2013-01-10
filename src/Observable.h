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
/*!
 * \file Observer.h
 * \brief Base class for observable classes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

namespace ea
{
	template<class T>
	class Observable
	{
		 public:
			virtual ~Observable() {}

			void attach_listener(T* l)
			{
				listener.push_back(l);
			}

			void detach_listener(T* l)
			{
				typename std::vector<T*>::iterator pos;
		
				pos = std::find(listener.begin(), listener.end(), l);

				if(pos != listener.end())
				{
					listener.erase(pos);
				}
			}
		
		protected:
			std::vector<T*> listener;
	};
}