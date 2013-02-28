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
   @file OnePointCrossover.h
   @brief Implementation of one-point crossover operator.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ONEPOINTCROSSOVER_H
#define ONEPOINTCROSSOVER_H

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
	   @class OnePointCrossover
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Implementation of the one-point crossover operator.
	 */
	template<class T>
	class OnePointCrossover : public ACrossover<T>
	{
		public:
			using ACrossover<T>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			OnePointCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<T>(rnd_generator) {}

			virtual ~OnePointCrossover() {};

			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, IInserter<AGenome<T>*>* inserter)
			{
				uint32_t separator;

				separator = (uint32_t)generator->get_number(1, a->size() - 2);

				inserter->insert(create_child(b, a, separator));
				inserter->insert(create_child(a, b, separator));

				return 2;
			}

		protected:
			using ACrossover<T>::generator;

		private:
			AGenome<T>* create_child(const AGenome<T>* a, const AGenome<T>* b, const uint32_t separator) const
			{
				uint32_t i;
				AGenome<T>* individual;
			
				individual = a->instance();

				for(i = 0; i < separator; ++i)
				{
					individual->copy_to(i, a->at(i));
				}

				for(i = separator; i < a->size(); ++i)
				{
					individual->copy_to(i, b->at(i));
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
