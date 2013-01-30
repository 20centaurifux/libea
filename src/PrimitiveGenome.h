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
/**
   @file PrimitiveGenome.h
   @brief A genome holding primitive datatypes.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef PRIMITIVEGENOME_H
#define PRIMITIVEGENOME_H

#include <vector>
#include <algorithm>
#include "AGenome.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class PrimitiveGenome
	   @tparam T gene datatype
	   @brief A genome for primitive datatypes like int, bool or float.
	 */
	template<class T>
	class PrimitiveGenome : public AGenome<T>
	{
		public:
			/**
			   @param size size of the genome
			   @param fitness_func functor to calculate the fitness of the genome
			 */
			PrimitiveGenome(const uint32_t size, const typename FitnessFunc<T>::fitness fitness_func)
				: AGenome<T>(size, fitness_func), genes(new std::vector<T>(size)), _modified(true), _fitness(0) {}

			virtual ~PrimitiveGenome()
			{
				delete genes;
			}

			float fitness()
			{
				if(_modified)
				{
					_fitness =  AGenome<T>::fitness_func(*this);
					_modified = false;
				}

				return _fitness;
			}

			void set(const uint32_t index, T gene)
			{
				(*genes)[index] = gene;
				_modified = true;
			}

			void copy_to(const uint32_t index, T gene)
			{
				set(index, gene);
				_modified = true;
			}

			T at(const uint32_t index) const
			{
				return (*genes)[index];
			}

			PrimitiveGenome* instance(const uint32_t size) const
			{
				return new PrimitiveGenome<T>(size, AGenome<T>::fitness_func);
			}

			bool find(T gene, uint32_t& index) const
			{
				typename std::vector<T>::iterator iter;

				iter = std::find(genes->begin(), genes->end(), gene);

				if(iter != genes->end())
				{
					index = iter - genes->begin();
					return true;
				}

				return false;
			}

			bool contains(T gene) const
			{
				if(std::find(genes->begin(), genes->end(), gene) == genes->end())
				{
					return false;
				}

				return true;
			}

			void swap(const uint32_t pos1, const uint32_t pos2) const
			{
				T temp;

				temp = (*genes)[pos1];
				(*genes)[pos1] = (*genes)[pos2];
				(*genes)[pos2] = temp;
			}

		protected:
			std::vector<T>* genes;

		private:
			bool _modified;
			float _fitness;
	};

	/**
	   @}
	 */
}
#endif
