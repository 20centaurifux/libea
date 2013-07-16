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
   @file PrimitiveGenome.h
   @brief A genome holding primitive datatypes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef PRIMITIVE_GENOME_H
#define PRIMITIVE_GENOME_H

#include <vector>
#include "SDBMHash.h"
#include "AGenome.h"
#include "join.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class PrimitiveGenome
	   @tparam TGene type of stored genes
	   @brief A genome holding primitive datatypes.
	 */
	template<typename TGene>
	class PrimitiveGenome : public AGenome<TGene, PrimitiveGenome<TGene>>
	{
		public:
			/*! Type of the fitness function related to the genome. */
			typedef double (*FitnessFunc)(const PrimitiveGenome<TGene>* genome);

			/**
			   @param size size of the genome
			   @param fitness a fitness function
			 */
			PrimitiveGenome(const uint32_t size, FitnessFunc fitness)
				: AGenome<TGene, PrimitiveGenome<TGene>>(size, fitness)
			{
				_genes = new TGene[size];
			}

			virtual ~PrimitiveGenome(void)
			{
				delete[] _genes;
			}

			TGene at(const uint32_t index) const override
			{
				return _genes[index];
			}

			void set(const uint32_t index, const TGene value) override
			{
				_genes[index] = value;
				this->modified();
			}

			void copy_to(const uint32_t index, const TGene value) override
			{
				_genes[index] = value;
				this->modified();
			}

			void swap(const uint32_t pos1, const uint32_t pos2) override
			{
				TGene temp;

				temp = _genes[pos1];
				_genes[pos1] = _genes[pos2];
				_genes[pos2] = temp;
				this->modified();
			}

			bool index_of(const TGene gene, uint32_t &index) const override
			{
				for(uint32_t i = 0; i < this->size(); i++)
				{
					if(_genes[i] == gene)
					{
						index = i;

						return true;
					}
				}

				return false;
			}

		protected:
			size_t hash_impl() override
			{
				AHash* hasher = &_hasher;

				hasher->reset();

				std::for_each(_genes, _genes + this->size(), [hasher](TGene gene) { (*hasher) << gene; });

				return hasher->hash();
			}

			std::string to_string_impl() override
			{
				return join<TGene>(_genes, _genes + this->size(), ",");
			}

		private:
			TGene* _genes;
			SDBMHash _hasher;
	};

	/// @cond INTERNAL
	template<>
	class PrimitiveGenome<bool> : public AGenome<bool, PrimitiveGenome<bool>>
	{
		public:
			/**
			   @param size size of the genome
			   @param fitness a fitness function
			 */
			PrimitiveGenome(const uint32_t size, FitnessFunc fitness)
				: AGenome<bool, PrimitiveGenome<bool>>(size, fitness)
			{
				_genes = new std::vector<bool>(size);
			}

			virtual ~PrimitiveGenome(void)
			{
				delete _genes;
			}

			bool at(const uint32_t index) const
			{
				return (*_genes)[index];
			}

			void set(const uint32_t index, const bool value)
			{
				(*_genes)[index] = value;
				modified();
			}

			void copy_to(const uint32_t index, const bool value)
			{
				(*_genes)[index] = value;
				modified();
			}

			void swap(const uint32_t pos1, const uint32_t pos2)
			{
				bool temp;

				temp = (*_genes)[pos1];
				(*_genes)[pos1] = (*_genes)[pos2];
				(*_genes)[pos2] = temp;
				modified();
			}

			bool index_of(const bool gene, uint32_t &index) const override
			{
				auto iter = std::find(_genes->begin(), _genes->end(), gene);

				if(iter != _genes->end())
				{
					index = iter - _genes->begin();
					return true;
				}

				return false;
			}

			/**
			   @param index position of a bit to flip

			   Flips a bit.
			 */
			void flip(const uint32_t index)
			{
				(*_genes)[index].flip();
				modified();
			}

			/**
			   @param text text to parse (e.g. "0010010")

			   Parses a text & overrides the genome.
			 */
			void parse_string(const std::string text)
			{
				uint32_t i = 0;

				for(char c : text)
				{
					if(i < this->size())
					{
						(*_genes)[i++] = c == '1';
					}
					else
					{
						break;
					}
				}
			}

		protected:
			size_t hash_impl() override
			{
				AHash* hasher = &_hasher;

				hasher->reset();

				std::for_each(begin(*_genes), end(*_genes), [hasher](bool gene) { (*hasher) << gene; });

				return hasher->hash();
			}

			std::string to_string_impl() override
			{
				return join<bool>(_genes->begin(), _genes->end(), ",");
			}

		private:
			std::vector<bool>* _genes;
			SDBMHash _hasher;
	};
	/// @endcond

	/**
	   @}
	 */
}

#endif
