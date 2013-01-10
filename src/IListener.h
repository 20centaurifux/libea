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

#ifndef ILISTENER_H
#define ILISTENER_H
namespace ea
{
	 class IListener
	 {
		 public:
		 	class EventArg
			{
				public:
					virtual ~EventArg() {}
			};

		 	virtual ~IListener() {}
	 };
 }
#endif
