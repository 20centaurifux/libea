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
   @file DoubleSwapMutation.hpp
   @brief Mutation operator swapping three genes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef DOUBLESWAPMUTATION_H
#define DOUBLESWAPMUTATION_H

#include <memory>
#include <cassert>
#include "AMutation.hpp"
#include "TR1UniformDistribution.hpp"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class DoubleSwapMutation
	   @tparam TGenomeBase a genome base class
	   @brief A mutation operator swapping three genes.
	 */
	template<class TGenomeBase>
	class DoubleSwapMutation : public AMutation<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/**
			   @param rnd instance of a random number generator
			 */
			DoubleSwapMutation(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}

			DoubleSwapMutation()
			{
				_rnd = std::make_shared<TR1UniformDistribution<>>();
			}

			virtual ~DoubleSwapMutation() {}

			void mutate(sequence_type& sequence)
			{
				static TGenomeBase base;
				int32_t offsets[3];

				assert(base.len(sequence) > 2);

				_rnd->get_unique_int32_seq(0, base.len(sequence) - 1, offsets, 3);

				auto seq = base.get(sequence, offsets[0]);
				base.set(sequence, offsets[0], base.get(sequence, offsets[1]));
				base.set(sequence, offsets[1], base.get(sequence, offsets[2]));
				base.set(sequence, offsets[2], seq);
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
