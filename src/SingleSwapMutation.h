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
   @file SingleSwapMutation.h
   @brief Mutation operator swapping two genes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef SINGLESWAPMUTATION_H
#define SINGLESWAPMUTATION_H

#include "AMutation.h"
#include <memory>
#include <cassert>
#include "TR1UniformDistribution.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class SingleSwapMutation
	   @tparam TGenomeBase a genome base class
	   @brief A mutation operator swapping two genes.
	 */
	template<class TGenomeBase>
	class SingleSwapMutation : public AMutation<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			SingleSwapMutation()
			{
				_rnd = std::make_shared<TR1UniformDistribution<std::mt19937_64>>();
			}

			/**
			   @param rnd instance of a random number generator
			 */
			SingleSwapMutation(std::shared_ptr<ARandomNumberGenerator> rnd)
			{
				assert(rnd != nullptr);
				_rnd = rnd;
			}

			virtual ~SingleSwapMutation() {}

			void mutate(sequence_type& sequence)
			{
				static TGenomeBase base;
				int32_t offsets[2];

				assert(base.len(sequence) > 1);

				_rnd->get_unique_int32_seq(0, base.len(sequence) - 1, offsets, 2);

				auto seq = base.get(sequence, offsets[0]);
				base.set(sequence, offsets[0], base.get(sequence, offsets[1]));
				base.set(sequence, offsets[1], seq);
			}

		private:
			std::shared_ptr<ARandomNumberGenerator> _rnd;
	};

	/**
		   @}
	   @}
	 */
}
#endif
