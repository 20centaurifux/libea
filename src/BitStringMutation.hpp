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
   @file BitStringMutation.hpp
   @brief Mutation operator flipping at least one random gene.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef BITSTRINGMUTATION_H
#define BITSTRINGMUTATION_H

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
	   @class BitStringMutation
	   @tparam TGenomeBase a genome base class
	   @tparam P prohability (0..100) to flip a gene
	   @brief A mutation operator flipping at least one random gene.
	 */
	template<class TGenomeBase, const int32_t P = 50>
	class BitStringMutation : public AMutation<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			BitStringMutation()
			{
				_rnd = std::make_shared<TR1UniformDistribution<>>();
			}

			/**
			   @param rnd instance of a random number generator
			 */
			BitStringMutation(std::shared_ptr<ARandomNumberGenerator> rnd)
			{
				assert(rnd != nullptr);
				_rnd = rnd;
			}

			virtual ~BitStringMutation() {}

			void mutate(sequence_type& sequence) override
			{
				static TGenomeBase base;
				int32_t* prohabilities;
				sequence_len_t len;
				bool flipped = false;

				len = base.len(sequence);

				assert(P >= 1 && P <= 100);
				assert(len > 0);

				prohabilities = new int32_t[len];

				while(!flipped)
				{
					_rnd->get_int32_seq(1, 100, prohabilities, len);

					for(sequence_len_t i = 0; i < len; ++i)
					{
						if(prohabilities[i] <= P)
						{
							base.set(sequence, i, !base.get(sequence, i));
							flipped = true;
						}
					}
				}

				delete[] prohabilities;
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
