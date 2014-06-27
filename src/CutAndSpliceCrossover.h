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
   @file CutAndSpliceCrossover.h
   @brief Implementation of the cut-and-splice crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef CUTANDSPLICECROSSOVER_H
#define CUTANDSPLICECROSSOVER_H

#include <cassert>
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
	   @class CutAndSpliceCrossover
	   @tparam TGenome type of the genome class
	   @brief Implementation of the cut-and-splice crossover operator.
	 */
	template<typename TGenome>
	class CutAndSpliceCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			CutAndSpliceCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}

			virtual ~CutAndSpliceCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				uint32_t separator1;
				uint32_t separator2;
				std::shared_ptr<TGenome> individual;
				uint32_t i;
				uint32_t m;
				uint64_t len;

				assert(a->size() >= 4);
				assert(a->size() < INT32_MAX);
				assert(b->size() >= 4);
				assert(b->size() < INT32_MAX);

				separator1 = (uint32_t)generator->get_int32(1, a->size() - 2);
				separator2 = (uint32_t)generator->get_int32(1, b->size() - 2);

				// create first individual:
				len = separator1 + b->size() - separator2;
				individual = std::make_shared<TGenome>(len, a->get_fitness_func());

				for(i = 0; i < separator1; i++)
				{
					individual->copy_to(i, a->at(i));
				}

				m = separator1;

				for(i = separator2; i < b->size(); i++)
				{
					individual->copy_to(m++, b->at(i));
				}

				output.append(individual);

				// create second individual:
				len = separator2 + a->size() - separator1;
				individual = std::make_shared<TGenome>(len, a->get_fitness_func());

				for(i = 0; i < separator2; i++)
				{
					individual->copy_to(i, b->at(i));
				}

				m = separator2;

				for(i = separator1; i < a->size(); i++)
				{
					individual->copy_to(m++, a->at(i));
				}

				output.append(individual);

				return 2;
			}

		protected:
			using ACrossover<TGenome>::generator;
	};

	/**
		   @}
	   @}
	 */
}
#endif
