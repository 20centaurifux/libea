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
   @file CutAndSpliceCrossover.h
   @brief Implementation of cut and splice crossover operator.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef CUTANDSPLICECROSSOVER_H
#define CUTANDSPLICECROSSOVER_H

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Crossover
		@{
	 */

	/**
	   @class CutAndSpliceCrossover
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Implementation of the cut and splice crossover operator.
	 */
	template<class T>
	class CutAndSpliceCrossover : public ACrossover<T>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			CutAndSpliceCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover<T>(rnd_generator) {}

			virtual ~CutAndSpliceCrossover() {};

			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, std::vector<AGenome<T>*>& children)
			{
				uint32_t separator1;
				uint32_t separator2;
				AGenome<T> *individual;
				uint32_t i;
				uint32_t m;

				assert(a != NULL);
				assert(a->size() > 1);
				assert(b != NULL);
				assert(b->size() > 1);

				m = separator1 = (uint32_t)ACrossover<T>::generator->get_number(1, a->size() - 2);
				separator2 = (uint32_t)ACrossover<T>::generator->get_number(1, a->size() - 2);

				// create first individual:
				individual = a->instance(separator1 + b->size() - separator2);

				for(i = 0; i < separator1; i++)
				{
					individual->copy_to(i, a->at(i));
				}

				for(i = separator2; i < b->size(); i++)
				{
					individual->copy_to(m++, b->at(i));
				}

				children.push_back(individual);

				// create second individual:
				individual = a->instance(separator2 + a->size() - separator1);

				for(i = 0; i < separator2; i++)
				{
					individual->copy_to(i, b->at(i));
				}

				m = separator2;

				for(i = separator1; i < a->size(); i++)
				{
					individual->copy_to(m++, a->at(i));
				}

				children.push_back(individual);

				return 2;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
