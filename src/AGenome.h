/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: sebastian.fedrau@gmail.com
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
 ***************************************************************************/
/**
   @file AGenome.h
   @brief Base class for genomes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef AGENOME_H
#define AGENOME_H

#include <memory>
#include <cstdint>
#include <string>
#include <cassert>
#include "InputAdapter.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	    @param src a genome to copy genes from
	    @param dst a genome to copy genes to
	    @tparam TGenome datatype of the genome
	  
	    Copies a genome to another one.
	 */
	template<typename TGenome>
	void copy(const TGenome& src, TGenome& dst)
	{
		assert(src->size() == dst->size());

		for(uint32_t i = 0; i < src->size(); i++)
		{
			dst->copy_to(i, src->at(i));
		}
	}

	/**
	   @class AGenome
	   @tparam TGene gene datatype
	   @tparam TDerived type of the derived class
	   @brief Abstract base class for genomes.
	 */
	template<typename TGene, typename TDerived>
	class AGenome
	{
		public:
			/*! Datatype of stored genes. */
			typedef TGene value_type;

			/*! Type of the fitness function related to the genome. */
			typedef double (*FitnessFunc)(const TDerived* genome);

			/**
			   @param size size of the genome
			   @param fitness a fitness function
			 */
			AGenome(const uint32_t size, FitnessFunc fitness)
				: _fitness_func(fitness), _size(size), _fitness_cached(false), _hash_cached(false), _str_cached(false) {}

			virtual ~AGenome(void) {}

			/**
			   @return size of the genome

			   Gets the size of the gnome.
			 */
			uint32_t size() const { return _size; };

			/**
			   @param index a location
			   @return a gene

			   Returns gene at the given position.
			 */
			virtual TGene at(const uint32_t index) const = 0;

			/**
			   @return the fitness function

			   Gets the fitness assigned function.
			 */
			FitnessFunc get_fitness_func()
			{
				return _fitness_func;
			}

			/**
			   @return the fitness value of the genome

			   Returns fitness of the genome. The fitness value is cached internally.
			 */
			double fitness()
			{
				if(!_fitness_cached)
				{
					_fitness = _fitness_func(reinterpret_cast<const TDerived*>(this));
					_fitness_cached = true;
				}

				return _fitness;
			}

			/**
			   @param index a position
			   @param gene gene to set

			   Sets the gene at the given position.
			 */
			virtual void set(const uint32_t index, const TGene gene) = 0;

			/**
			   @param index a position
			   @param gene gene to copy

			   Copies a gene to the specified position. If you store object pointers you might
			   want to use this function to clone the object and free memory.
			 */
			virtual void copy_to(const uint32_t index, const TGene gene) = 0;

			/**
			   @param pos1 position of the first gene
			   @param pos2 position of the second gene

			   Swaps two genes.
			 */
			virtual void swap(const uint32_t pos1, const uint32_t pos2) = 0;

			/**
			   @param gene gene to search
			   @param index reference to uint32_t to store the found index
			   @return true if gene could be found

			   Searches for a gene.
			 */
			virtual bool index_of(const TGene gene, uint32_t &index) const = 0;

			/**
			   @return a string

			   Returns a string representing the object.
			 */
			std::string to_string()
			{
				if(!_str_cached)
				{
					_str = to_string_impl();
					_str_cached = true;
				}

				return _str;
			}


			/**
			   @return hash of the genome

			   Returns hash of the genome. The hash value is cached internally.
			 */
			size_t hash()
			{
				if(!_hash_cached)
				{
					_hash = hash_impl();
					_hash_cached = true;
				}

				return _hash;
			}

		protected:
			/**
			   @return hash of the genome

			   Returns hash of the genome.
			 */
			virtual size_t hash_impl() = 0;

			/**
			   @return a string

			   Returns a string representing the object.
			 */
			virtual std::string to_string_impl() = 0;

			/*! Called from derived class to reset cached fitness & hash value. */
			void modified()
			{
				_fitness_cached = false;
				_hash_cached = false;
				_str_cached = false;
			}

		private:
			FitnessFunc _fitness_func;
			uint32_t _size;
			bool _fitness_cached;
			double _fitness;
			bool _hash_cached;
			size_t _hash;
			bool _str_cached;
			std::string _str;
	};

	/**
	   @}
	 */
}
#endif
