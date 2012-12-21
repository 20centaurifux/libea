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
 * \file ISequence.h
 * \brief Interface for sequences.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef ISEQUENCE_H
#define ISEQUENCE_H

namespace ea
{
	template<class T>
	class ISequence
	{
		public:
			ISequence() {}
			virtual ~ISequence() {}
			virtual bool has_next() const = 0;
			virtual T get_item() const = 0;
			virtual bool next() = 0;
			virtual bool contains(T object) = 0;
	};
}
#endif
