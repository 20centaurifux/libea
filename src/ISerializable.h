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
 * \file ISerializable.h
 * \brief Interface for classes which can be serialized.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include "ea.h"

namespace ea
{
	class ISerializable
	{
		public:
			virtual void read(const uint32_t offset, byte* memory, const uint32_t count) const = 0;
			virtual byte read_byte(const uint32_t offset) const = 0;
			virtual int32_t read_int32(const uint32_t offset) const = 0;
			virtual inline uint32_t read_uint32(const uint32_t offset) const = 0;

		protected:
			virtual ~ISerializable() {}
	};
}
#endif
