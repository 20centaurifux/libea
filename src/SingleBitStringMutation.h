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
   @file SingleBitStringMutation.h
   @brief Mutation operator flipping a random gene.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef SINGLEBITSTRINGMUTATION_H
#define SINGLEBITSTRINGMUTATION_H

#include <memory>
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
	   @class SingleBitStringMutation
	   @tparam TGenomeBase a genome base class
	   @brief A mutation operator flipping a random gene.
	 */
	template<class TGenomeBase>
	class SingleBitStringMutation : public AMutation<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			SingleBitStringMutation()
			{
				_rnd = std::make_shared<TR1UniformDistribution<std::mt19937_64>>();
			}

			/**
			   @param rnd instance of a random number generator
			 */
			SingleBitStringMutation(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}
			virtual ~SingleBitStringMutation() {}

			void mutate(sequence_type& sequence) override
			{
				static TGenomeBase base;
				int32_t index;

				assert(base.len(sequence) > 0);

				index = _rnd->get_int32(0, base.len(sequence) - 1);

				base.set(sequence, index, !base.get(sequence, index));
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
