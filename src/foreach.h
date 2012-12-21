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
 * \file foreach.h
 * \brief foreach template function.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef FOREACH_H
#define FOREACH_H

#include <vector>
#include "IEquatable.h"

namespace ea
{
	template<class T>
	void delete_object(T*& obj)
	{
		delete obj;
		obj = NULL;
	}

	template<class T, class F>
	F foreach(std::vector<T>& collection, F f)
	{
		for(uint32_t i = 0; i < collection.size(); i++)
		{
			f(collection.at(i));
		}

		return f;
	}
}
#endif
