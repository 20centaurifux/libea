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
   @file UniformCrossover.h
   @brief Implementation of the uniform crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef UNIFORMCROSSOVER_H
#define UNIFORMCROSSOVER_H

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
	   @class UniformCrossover
	   @tparam TGenome type of the genome class
	   @tparam N Ratio between two parents
	   @brief Implementation of the uniform crossover operator.
	 */
	template<typename TGenome, const int32_t N = 2>
	class UniformCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			UniformCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}

			virtual ~UniformCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				std::shared_ptr<TGenome> child0;
				std::shared_ptr<TGenome> child1;
				int32_t rnd;

				assert(a->size() > N);
				assert(a->size() == b->size());

				child0 = std::make_shared<TGenome>(a->size(), a->get_fitness_func());
				child1 = std::make_shared<TGenome>(a->size(), a->get_fitness_func());

				for(uint32_t i = 0; i < a->size(); i++)
				{
					do
					{
						rnd = generator->get_int32();
					} while(!rnd);

					if(rnd % N)
					{
						child0->copy_to(i, a->at(i));
						child1->copy_to(i, b->at(i));
					}
					else
					{
						child0->copy_to(i, b->at(i));
						child1->copy_to(i, a->at(i));
					}
				}

				output.append(child0);
				output.append(child1);

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
