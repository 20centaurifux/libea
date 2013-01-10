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
 * \file Genome.h
 * \brief A genome holds multiple genes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include "Gene.h"
#include "IHashfunction.h"

namespace ea
{
	class Genome
	{
		public:
			Genome();
			Genome(const uint32_t size);
			virtual ~Genome();
			Gene* append_gene(Gene* gene);
			Gene* append_new_gene(const int32_t length);
			void set_gene(const uint32_t index, Gene* gene);
			Gene* gene_at(const uint32_t index) const;
			void remove_gene(const uint32_t index);
			inline uint32_t size() const { return _genes->size(); }
			int32_t find_gene(const Gene* gene) const;
			size_t hash() const;

		private:
			std::vector<Gene*>* _genes;
			mutable size_t _hash;
			mutable bool _hash_set;
	};
}
#endif
