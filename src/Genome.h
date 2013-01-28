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
 * \file main.cpp
 * \brief A genome holds objects inherited from the AGene base class.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef GENOME_H
#define GENOME_H

#include <algorithm>
#include "AGenome.h"
#include "AGene.h"

namespace ea
{
	class Genome : public AGenome<AGene*>, public AGeneListener
	{
		public:
			Genome(const uint32_t size, const FitnessFunc<AGene*>::fitness fitness_func)
				: AGenome(size, fitness_func), _genes(new std::vector<AGene*>(size)), _modified(true), _fitness(0)
			{
				std::fill(_genes->begin(), _genes->end(), nullptr);
			}

			virtual ~Genome()
			{
				std::for_each(_genes->begin(), _genes->end(), [] (AGene* gene) { if(gene) delete gene; });
				delete _genes;
			}

			float fitness();
			void set(const uint32_t index, AGene* gene);
			void copy_to(const uint32_t index, AGene* gene);
			AGene* at(const uint32_t index) const;
			Genome* instance(const uint32_t size) const;
			bool find(AGene* gene, uint32_t& index) const;
			bool contains(AGene* gene) const;
			void swap(const uint32_t pos1, const uint32_t pos2) const;

			void modified(const AGene* gene)
			{
				_modified = true;
			}

		private:
			std::vector<AGene*>* _genes;
			bool _modified;
			float _fitness;
	};
}
#endif
