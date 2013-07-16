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
   @file Genome.h
   @brief A genome holds objects derived from the TGene base class.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */

#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <sstream>
#include "AGene.h"
#include "AGenome.h"
#include "SDBMHash.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class Genome
	   @brief A genome holding and observing TGene instances.
	 */
	template<typename TGene>
	class Genome: public AGenome<TGene*, Genome<TGene>>, public AGeneListener<TGene>
	{
		public:
			/**
			   @struct LessThan
			   @brief Functor for comparing two genomes.
			 */
			typedef struct
			{
				/**
				   @param a a gene
				   @param b a gene
				   @return true if a is smaller than b

				   Compares two genes.
				 */
				bool operator()(TGene* a, TGene* b)
				{
					if(a->size() == b->size())
					{
						for(uint32_t i = 0; i < a->size(); i++)
						{
							if(a->at(i)->less_than(b->at(i)))
							{
								return true;
							}
						}
					}
					else
					{
						return a->size() < b->size();
					}

					return false;
				}
			} LessThan;

			/*! Type of the fitness function related to the genome. */
			typedef double (*FitnessFunc)(const Genome<TGene>* genome);

			/**
			   @param size size of the genome
			   @param fitness functor to calculate the fitness of the genome
			 */
			Genome(const uint32_t size, FitnessFunc fitness)
				: AGenome<TGene*, Genome>(size, fitness)
			{
				_genes = new std::vector<TGene*>(size);
				std::fill(begin(*_genes), end(*_genes), nullptr);
			}

			virtual ~Genome()
			{
				std::for_each(begin(*_genes), end(*_genes), [](TGene* gene) { delete gene; });
				delete _genes;
			}

			TGene* at(const uint32_t index) const override
			{
				return (*_genes)[index];
			}

			/**
			   @param index a position
			   @param gene gene to set

			   Sets the gene at the given position.
			 */
			void set(const uint32_t index, TGene* const gene) override
			{
				(*_genes)[index] = gene;
				gene->attach_listener(this);
				this->modified();
			}

			/**
			   @param index a position
			   @param gene gene to copy

			   Copies a gene to the specified position. If you store object pointers you might
			   want to use this function to clone the object and free memory.
			 */
			void copy_to(const uint32_t index, TGene* const gene) override
			{
				TGene* g = gene->clone();

				if((*_genes)[index])
				{
					delete (*_genes)[index];
				}

				(*_genes)[index] = g;
				g->attach_listener(this);
				this->modified();
			}

			void swap(const uint32_t pos1, const uint32_t pos2) override
			{
				TGene* gene;

				if(pos1 == pos2)
				{
					return;
				}

				gene = (*_genes)[pos1];
				(*_genes)[pos1] = (*_genes)[pos2];
				(*_genes)[pos2] = gene;

				this->modified();
			}

			/**
			   @param gene gene to search
			   @param index reference to uint32_t to store the found index
			   @return true if gene could be found

			   Searches for a gene.
			 */
			bool index_of(TGene* const gene, uint32_t &index) const override
			{
				for(uint32_t i = 0; i < this->size(); i++)
				{
					if(!gene->less_than((*_genes)[i]) && !(*_genes)[i]->less_than(gene))
					{
						index = i;

						return true;
					}
				}

				return false;
			}

			void gene_modified(TGene* gene) override
			{
				this->modified();
			}

		protected:
			size_t hash_impl() override
			{
				SDBMHash hasher;

				std::for_each(begin(*_genes), end(*_genes), [&hasher](TGene* gene) { hasher << gene->hash(); });

				return hasher.hash();
			}

			std::string to_string_impl() override
			{
				std::ostringstream stream;
				auto first = begin(*_genes);

				stream << (*first)->to_string();
				std::for_each(first + 1, end(*_genes), [&stream](TGene* gene) { stream << "," << gene->to_string(); });

				return stream.str();
			}

		private:
			SDBMHash _hasher;
			std::vector<TGene*>* _genes;
	};

	/**
	   @}
	 */
}
#endif
