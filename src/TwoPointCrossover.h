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
   @file TwoPointCrossover.h
   @brief Implementation of two-point crossover operator.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef TWOPOINTCROSSOVER_H
#define TWOPOINTCROSSOVER_H

#include "ACrossover.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Crossover
		@{
	 */

	/**
	   @class TwoPointCrossover
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Implementation of the two-point crossover operator.
	 */
	template<class T>
	class TwoPointCrossover : public ACrossover<T>
	{
		public:
			using ACrossover<T>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			TwoPointCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover<T>(rnd_generator) {}

			virtual ~TwoPointCrossover() {};

			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, IInserter<AGenome<T>*>* inserter)
			{
				uint32_t offset1;
				uint32_t offset2;

				offset1 = (uint32_t)generator->get_number(1, a->size() - 3);
				offset2 = (uint32_t)generator->get_number(offset1 + 1, a->size() - 1);

				inserter->insert(crossover(b, a, offset1, offset2));
				inserter->insert(crossover(a, b, offset1, offset2));

				return 2;
			}

		protected:
			using ACrossover<T>::generator;

		private:
			AGenome<T>* crossover(const AGenome<T>* a, const AGenome<T>* b, const uint32_t offset1, const uint32_t offset2) const
			{
				uint32_t i;
				AGenome<T> *individual;
			
				individual = a->instance();

				for(i = 0; i < offset1; ++i)
				{
					individual->copy_to(i, a->at(i));
				}

				for(i = offset1; i < offset2; ++i)
				{
					individual->copy_to(i, b->at(i));
				}

				for(i = offset2; i < a->size(); ++i)
				{
					individual->copy_to(i, a->at(i));
				}

				return individual;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
