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
 * \file AGenome.h
 * \brief Genome base class.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <stdint.h>
#include <stddef.h>
#include <stdexcept>
#include "IHashfunction.h"

namespace ea
{
	 template<class T>
	 class AGenome : public IHashfunction
	 {
		public:
			AGenome(const uint32_t size) : _size(size) {} 
			virtual ~AGenome() {}
			inline uint32_t size() const { return _size; }
			virtual void set(const uint32_t index, T gene) = 0;
			virtual void copy_to(const uint32_t index, const T gene) = 0;
			virtual T at(const uint32_t index) const = 0;

			virtual uint32_t index_of(T gene) const
			{
				uint32_t index;

				if(find(gene, index))
				{
					return index;
				}

				throw std::out_of_range("index is out of range");
			}

			virtual bool find(T gene, uint32_t& index) const = 0;
			virtual bool contains(T gene) const = 0;
			virtual void swap(const uint32_t pos1, const uint32_t pos2) const = 0;
			virtual size_t hash() const = 0;

		private:
			uint32_t _size;
	 };
}
