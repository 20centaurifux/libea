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

#include <cassert>
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
	   @tparam T datatype of genes stored in the genome
	   @tparam Compare functor to compare fitness values
	   @brief Implementation of fitness proportional selection.
	 */
	template<class T, class Compare>
	class FitnessProportionalSelection : public AIndexSelection<T, Compare>
	{
		public:
			using AIndexSelection<T, Compare>::select;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			FitnessProportionalSelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: AIndexSelection<T, Compare>(rnd_generator) {}

			virtual ~FitnessProportionalSelection() {};

			void select(IIterator<AGenome<T>*> *iter, const uint32_t count, std::vector<uint32_t>& selection)
			{
				float* sums = new float[iter->size()];
				float max;
				uint32_t i;
				uint32_t range[2];

				sums[0] = iter->current()->fitness();

				for(i = 1; i < iter->size(); ++i)
				{
					sums[i] = sums[i - 1] + iter->current()->fitness();
				}

				max = sums[iter->size() - 1];
				assert(max != 0);

				while(selection.size() != count)
				{
					range[0] = 0;
					range[1] = iter->size() - 1;

					if(max > 0)
					{
						selection.push_back(find_index(sums, range, (uint32_t)generator->get_number(0, max)));
					}
					else
					{
						selection.push_back(find_index(sums, range, (uint32_t)generator->get_number(max, 0)));
					}
				}

				delete sums;
			}
			
		protected:
			using AIndexSelection<T, Compare>::generator;
			using AIndexSelection<T, Compare>::compare;

		private:
			uint32_t find_index(const float* sums, uint32_t range[2], const uint32_t n) const
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
