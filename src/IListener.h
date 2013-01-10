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
 * \file IListener.h
 * \brief Interface for observers.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <vector>
#include <algorithm>

namespace ea
{
	 template<class T, class E>
	 class IListener
	 {
		 public:
		 	class EventArg
			{
				public:
					virtual ~EventArg() {}
			};

		 	virtual ~IListener() {}
			virtual void created(const T* sender, const E* arg) = 0;
			virtual void modified(const T* sender, const E* arg) = 0 ;
			virtual void deleted(const T* sender, const E* arg) = 0;
	 };
 }
