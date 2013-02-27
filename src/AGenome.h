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
   @file AGenome.h
   @brief Base class for genomes.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef AGENOME_H
#define AGENOME_H

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <stdexcept>
#include <assert.h>

#include "IToString.h"
#include "join.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	template<class T>
	class AGenome;

	/**
	   @struct FitnessFunc
	   @tparam T gene datatype
	   @brief Functor to calculate the fitness of a genome.
	 */
	template<typename T>
	struct FitnessFunc
	{
		/*! Defines a fitness function. */
		typedef float (*fitness)(const AGenome<T>& obj);
	};

	/**
	   @class AGenome
	   @tparam T gene datatype
	   @brief Abstract base class for genomes.
	 */
	template<class T>
	class AGenome : public IToString
	{
		public:
			/**
			   @param size size of the genome
			   @param fitness_func functor to calculate the fitness of the genome
			 */
			AGenome(const uint32_t size, const typename FitnessFunc<T>::fitness fitness_func)
				: fitness_func(fitness_func), _size(size), _fitness_set(false) {} 

			virtual ~AGenome() {}

			/**
			   @param from genome to copy genes from
			   @param to genome to copy genes to

			   Copies genes from a genome to a different one.
			 */
			static void copy(const AGenome<T>& from, AGenome<T>& to)
			{
				assert(from.size() == to.size());

				for(uint32_t i = 0; i < from.size(); ++i)
				{
					to.copy_to(i, from.at(i));
				}
			}

			/**
			   @return size of the genome

			   Gets the size of the gnome.
			 */
			inline uint32_t size() const { return _size; }

			/**
			   @param index a position
			   @param gene gene to set

			   Sets the gene at the given position. If you store objects in the genome this method might
			   insert the given pointer into the genome.
			 */
			virtual void set(const uint32_t index, T gene) = 0;

			/**
			   @param index a position
			   @param gene gene to copy

			   Copies a gene to the given position. If you store objects in the genome this method might
			   create a new copy of the gene and insert the copy into the genome.
			 */
			virtual void copy_to(const uint32_t index, T gene) = 0;

			/**
			   @param index a location
			   @return a gene

			   Returns gene at the given position.
			 */
			virtual T at(const uint32_t index) const = 0;

			/**
			   @param size size of the new genome
			   @return a new gene

			   Creates a new uninitialized gene with the given size.
			 */
			virtual AGenome<T>* instance(const uint32_t size) const = 0;

			/**
			   @return a new gene

			   Creates a new uninitialized gene with the size with the same size.
			 */
			inline AGenome<T>* instance() const { return instance(_size); }

			/**
			   @return fitness of the genome

			   Calculates the fitness of the genome.
			 */
			virtual float fitness()
			{
				return fitness_func(*this);
			}

			/**
			   @param gene a gene
			   @return index of the given gene

			   Gets index of a gene.
			 */
			virtual uint32_t index_of(T gene) const
			{
				uint32_t index;

				if(find(gene, index))
				{
					return index;
				}

				throw std::out_of_range("index is out of range");
			}

			/**
			   @param gene a gene
			   @param index reference to uint32_t to store the found position
			   @return true if gene could be found

			   Searches for a gene.
			 */
			virtual bool find(T gene, uint32_t& index) const = 0;

			/**
			   @param gene a gene
			   @return true if gene could be found

			   Tests if genome contains the given gene.
			 */
			virtual bool contains(T gene) const = 0;

			/**
			   @param pos1 position of the first gene
			   @param pos2 position of the second gene

			   Swaps to genes.
			 */
			virtual void swap(const uint32_t pos1, const uint32_t pos2) const = 0;

			/**
			   @param separator a separator
			   @return a string

			   Returning a string representing the object.
			 */
			virtual std::string to_string(const std::string& separator) const = 0;

			/**
			   @return a string

			   Returning a string representing the object.
			 */
			virtual std::string to_string() = 0;

		protected:
			/*! Functor for calculating the fitness. */
			typename FitnessFunc<T>::fitness fitness_func;

		private:
			uint32_t _size;
			bool _fitness_set;
			float _fitness;
	};

	/**
	   @}
	 */
}
#endif
