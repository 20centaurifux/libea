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
   @file algorithms.h
   @brief Various algorithms.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <algorithm>
#include <set>
#include "AGenome.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @tparam TGenomeBase a genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @return true if given sequence is a multi-set

	   Tests if a sequence is a multi-set.
	  */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	bool is_multiset(const typename TGenomeBase::sequence_type& seq)
	{
		static TGenomeBase base;
		std::set<typename TGenomeBase::gene_type, LessThan> set;

		for(uint32_t i = 0; i < base.len(seq); i++)
		{
			const typename TGenomeBase::gene_type& gene = base.get(seq, i);

			if(set.find(gene) != end(set))
			{
				return false;
			}

			set.insert(gene);
		}

		return true;
	}

	/**
	   @tparam TGenomeBase a genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @return true if both sequences are equal.

	   Tests if two sequences are qual. The sequences have to be multi-sets.
	  */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	bool set_equals(const typename TGenomeBase::sequence_type& a, const typename TGenomeBase::sequence_type& b)
	{
		assert(is_multiset<TGenomeBase>(a));
		assert(is_multiset<TGenomeBase>(b));

		static TGenomeBase base;
		uint32_t len = base.len(a);

		if(len != base.len(b))
		{
			return false;
		}

		for(uint32_t i = 0; i < len; i++)
		{
			if(base.index_of(a, base.get(b, i)) == -1)
			{
				return false;
			}
		}

		return true;
	}


	/**
	   @}
	 */
}
#endif
