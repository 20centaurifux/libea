/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
T   it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/*!
 * \file main.cpp
 * \brief Genome holding primitive datatypes as genes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef PRIMITIVEGENOME_H
#define PRIMITIVEGENOME_H

#include <vector>
#include <algorithm>
#include "AGenome.h"

namespace ea
{
	template<class T, class Hash>
	class PrimitiveGenome : public AGenome<T>
	{
		public:
			PrimitiveGenome(const uint32_t size) : AGenome<T>(size),  _genes(new vector<T>(size)) {}

			virtual ~PrimitiveGenome()
			{
				delete _genes;
			}

			inline uint32_t size() const { return _genes->size(); }

			void set(const uint32_t index, T gene)
			{
				(*_genes)[index] = gene;
			}

			void copy_to(const uint32_t index, T gene)
			{
				set(index, gene);
			}

			T at(const uint32_t index) const
			{
				return (*_genes)[index];
			}

			bool find(T gene, uint32_t& index) const
			{
				typename std::vector<T>::iterator iter;

				iter = std::find(_genes->begin(), _genes->end(), gene);

				if(iter != _genes->end())
				{
					index = iter - _genes->begin();
					return true;
				}

				return false;
			}

			bool contains(T gene) const
			{
				if(std::find(_genes->begin(), _genes->end(), gene) == _genes->end())
				{
					return false;
				}

				return true;
			}

			void swap(const uint32_t pos1, const uint32_t pos2) const
			{
				T temp;

				temp = (*_genes)[pos1];
				(*_genes)[pos1] = (*_genes)[pos2];
				(*_genes)[pos2] = temp;
			}

		private:
			std::vector<T>* _genes;
			Hash _hash;

	};
}
#endif
