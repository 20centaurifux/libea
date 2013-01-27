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
 * \brief Base class for genomes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef AGENOME_H
#define AGENOME_H

#include <stdint.h>
#include <stddef.h>
#include <stdexcept>
#include "IHashfunction.h"

namespace ea
{
	template<class T>
	class AGenome;

	template<typename T>
	struct FitnessFunc
	{
		typedef float (*fitness)(const AGenome<T>& obj);
	};

	 template<class T>
	 class AGenome
	 {
		public:
			AGenome(const uint32_t size, const typename FitnessFunc<T>::fitness fitness_func)
				: fitness_func(fitness_func), _size(size), _fitness_set(false) {} 
			virtual ~AGenome() {}
			inline uint32_t size() const { return _size; }
			virtual void set(const uint32_t index, T gene) = 0;
			virtual void copy_to(const uint32_t index, T gene) = 0;
			virtual T at(const uint32_t index) const = 0;
			virtual AGenome<T>* instance(const uint32_t size) const = 0;
			inline AGenome<T>* instance() const { return instance(_size); }

			virtual float fitness()
			{
				return fitness_func(*this);
			}

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

		protected:
			typename FitnessFunc<T>::fitness fitness_func;

		private:
			uint32_t _size;
			bool _fitness_set;
			float _fitness;
	 };
}
#endif
