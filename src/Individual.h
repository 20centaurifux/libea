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
 * \file Individual.h
 * \brief An individual holds a genome and a fitness function.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "Genome.h"

namespace ea
{
	class Individual;

	typedef float (*FitnessFunc)(Individual* individual);

	class Individual : public Genome, public GenomeListener
	{
		public:
			Individual(const FitnessFunc fitness, uint32_t size);
			virtual ~Individual() {}
			float fitness();
			inline FitnessFunc get_fitness_func() const { return _fitness_func; }

			inline void created(const Genome* sender, const GenomeEventArg* arg) { _fitness_set = false; }
			inline void modified(const Genome* sender, const GenomeEventArg* arg) { _fitness_set = false; }
			inline void deleted(const Genome* sender, const GenomeEventArg* arg) { _fitness_set = false; }
			inline void modified(const Gene* sender, const GeneEventArg* arg) { _fitness_set = false; }
			inline void cleared(const Gene* sender) { _fitness_set = false; }

		private:
			FitnessFunc _fitness_func;
			bool _fitness_set;
			float _fitness;
			void init(const FitnessFunc fitness);
	};
}
#endif
