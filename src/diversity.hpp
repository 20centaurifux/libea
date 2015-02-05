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
   @file diversity.hpp
   @brief Various algorithms to determine the diversity of a population.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef DIVERSITY_H
#define DIVERSITY_H

#include <cassert>
#include <cmath>
#include <algorithm>
#include <map>
#include "InputAdapter.hpp"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @tparam TGenomeBase genome base class
	   @param source a population
	   @return the sequence length

	   Tests if all sequences of a population have the same length (if assertion is enabled) and returns it.
	 */
	template<typename TGenomeBase>
	static uint32_t _get_seq_len(IInputAdapter<typename TGenomeBase::sequence_type>& source)
	{
		static TGenomeBase base;
		uint32_t len;

		assert(source.size() > 0);

		source.first();

		if(!source.end())
		{
			len = base.len(source.current());
		}

		// test if all sequences have same length:
		#ifndef NDEBUG
		source.next();

		while(!source.end())
		{
			assert(base.len(source.current()) == len);
			source.next();
		}
		#endif

		return len;
	}

	/**
	   @tparam TGenomeBase genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @param a first sequence
	   @param b second sequence
	   @return the hamming distance

	   @brief Computes the hamming distance between two sequences.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	uint32_t hamming_distance(const typename TGenomeBase::sequence_type& a, const typename TGenomeBase::sequence_type& b)
	{
		static TGenomeBase base;
		LessThan lt;
		uint32_t d = 0;

		assert(base.len(a) == base.len(b));

		for(uint32_t i = 0; i < base.len(a); ++i)
		{
			if(lt(base.get(a, i), base.get(b, i)) || lt(base.get(b, i), base.get(a, i)))
			{
				++d;
			}
		}

		return d;
	}

	/**
	   @tparam TGenomeBase genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @param source a population
	   @return the hamming distance

	   @brief Computes the average hamming distance of a population.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	double distance_diversity(IInputAdapter<typename TGenomeBase::sequence_type>& source)
	{
		static TGenomeBase base;
		uint32_t len;
		double d = 0.0;

		assert(len >= 2);

		len = _get_seq_len<TGenomeBase>(source);

		for(uint32_t i = 0; i < source.size(); ++i)
		{
			for(uint32_t j = 0; j < source.size(); ++j)
			{
				if(i != j)
				{
					d += hamming_distance<TGenomeBase>(source.at(i), source.at(j));
				}
			}
		}

		d /= source.size() * (source.size() - 1);

		return d;
	}

	/**
	   @enum LOGBase
	   \brief Specifies the base to a logarithm.
	 */
	typedef enum
	{
		/*! Base 2. */
		LOGBASE_2,
		/*! Base 10. */
		LOGBASE_10,
		/*! Euler's number. */
		LOGBASE_E
	} LOGBase;

	/**
	   @tparam TGenomeBase genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @param source a population
	   @return index of the fittest genome

	   @brief Finds the fittest genome of a population.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	double shannon_diversity(IInputAdapter<typename TGenomeBase::sequence_type>& source, LOGBase lbase = LOGBASE_E)
	{
		TGenomeBase base;
		uint32_t len;
		std::map<typename TGenomeBase::gene_type, uint32_t, LessThan> map;
		double entropy = 0.0;

		len = _get_seq_len<TGenomeBase>(source);

		for(uint32_t k = 0; k < len; ++k)
		{
			source.first();
			map.clear();

			while(!source.end())
			{
				auto gene = base.get(source.current(), k);
				auto iter = map.find(gene);

				if(iter == map.end())
				{
					map[gene] = 1;
				}
				else
				{
					map[gene] = map[gene] + 1;
				}

				source.next();
			}

			for(auto g : map)
			{
				auto f = (double)g.second / source.size();

				if(std::fpclassify(f) != FP_ZERO)
				{
					switch(lbase)
					{
						case LOGBASE_2:
							entropy += -f * std::log2(f);
							break;

						case LOGBASE_10:
							entropy += -f * std::log10(f);
							break;

						default:
							entropy += -f * std::log(f);
					}
				}
			}
		}

		entropy *= (double)1 / len;

		return entropy;
	}

	/**
	   @}
	 */
}
#endif
