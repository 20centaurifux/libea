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
   @file FitnessProportionalSelection.h
   @brief Implementation of fitness proportional selection.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef FITNESSPROPORTIONALSELECTION_H
#define FITNESSPROPORTIONALSELECTION_H

#include "AIndexSelection.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Selection
		@{
	 */

	/**
	   @class FitnessProportionalSelection
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Implementation of fitness proportional selection.
	 */
	template<class T>
	class FitnessProportionalSelection : AIndexSelection<T>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			FitnessProportionalSelection(ARandomNumberGenerator* rnd_generator)
				: AIndexSelection<T>(rnd_generator) {}

			virtual ~FitnessProportionalSelection() {};

			void select(const std::vector<AGenome<T>*>& population, const uint32_t count, std::vector<uint32_t>& selection)
			{
				float* sums = new float[population.size()];
				float max;
				uint32_t i;
				uint32_t range[2];
				uint32_t index;

				sums[0] = population.at(0)->fitness();

				for(i = 1; i < population.size(); ++i)
				{
					sums[i] = sums[i - 1] + population.at(i)->fitness();
				}

				max = sums[population.size() - 1];

				while(selection.size() != count)
				{
					do
					{
						range[0] = 0;
						range[1] = population.size() - 1;

						index = find_index(sums, range, (uint32_t)AIndexSelection<T>::generator->get_number(1, max));
					} while(std::find(selection.begin(), selection.end(), index) != selection.end());

					selection.push_back(index);
				}

				delete sums;
			}

		private:
			inline uint32_t find_index(const float* sums, uint32_t range[2], const uint32_t n) const
			{
				uint32_t mid;

				while(range[1] - range[0] > 1)
				{
					mid = (range[1] - range[0]) / 2 + range[0];

					if(sums[mid] > n)
					{
						range[1] = mid;
					}
					else
					{
						range[0] = mid;
					}
				}

				return range[0];
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
