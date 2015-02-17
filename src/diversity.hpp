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
   @brief Various algorithms to compute the diversity of a population.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef DIVERSITY_H
#define DIVERSITY_H

#include <cassert>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>
#include "InputAdapter.hpp"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/*
	 * test if all sequences of a population have the same length (if assertion is enabled) and return it:
	 */
	template<typename TGenomeBase>
	static sequence_len_t _get_seq_len(IInputAdapter<typename TGenomeBase::sequence_type>& source)
	{
		static TGenomeBase base;
		sequence_len_t len;

		assert(source.size() > 0);

		source.first();
		len = base.len(source.current());

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

	/*
	 * hamming distance:
	 */

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
		static LessThan lt;
		uint32_t d = 0;

		assert(base.len(a) == base.len(b));

		for(sequence_len_t i = 0; i < base.len(a); ++i)
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

	   @brief Computes the average hamming distance between all distances of a population.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	double distance_diversity(IInputAdapter<typename TGenomeBase::sequence_type>& source)
	{
		static TGenomeBase base;
		sequence_len_t len;
		double d = 0.0;

		assert(len >= 2);

		len = _get_seq_len<TGenomeBase>(source);

		for(std::size_t i = 0; i < source.size(); ++i)
		{
			for(std::size_t j = 0; j < source.size(); ++j)
			{
				if(i != j)
				{
					d += hamming_distance<TGenomeBase, LessThan>(source.at(i), source.at(j));
				}
			}
		}

		d /= source.size() * (source.size() - 1);

		return d;
	}

	/*
	 * shannon diversity:
	 */

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
	   @return the shannon entropy

	   @brief Computes the shannon entropy of a population.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	double shannon_diversity(IInputAdapter<typename TGenomeBase::sequence_type>& source, LOGBase lbase = LOGBASE_E)
	{
		static TGenomeBase base;
		sequence_len_t len;
		std::map<typename TGenomeBase::gene_type, uint32_t, LessThan> map;
		double entropy = 0.0;

		len = _get_seq_len<TGenomeBase>(source);

		for(sequence_len_t k = 0; k < len; ++k)
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

	/*
	 * substring diversity:
	 */

	/*
	 * test if two substrings are equal:
	 */
	template<typename LessThan, typename TGenomeBase>
	static bool _cmp_substr(const TGenomeBase& base,
	                        const LessThan& lt,
	                        const typename TGenomeBase::sequence_type& seqx,
	                        const sequence_len_t x0, const sequence_len_t x1, 
	                        const typename TGenomeBase::sequence_type& seqy,
	                        const sequence_len_t y0, const sequence_len_t y1)
	{
		assert(x1 >= x0);
		assert(y1 >= y0);

		if(x1 - x0 != y1 - y0)
		{
			return false;
		}

		for(sequence_len_t i = 0; i <= y1 - y0; ++i)
		{
			if(lt(base.get(seqx, x0 + i), base.get(seqy, y0 + i)) || lt(base.get(seqy, y0 + i), base.get(seqx, x0 + i)))
			{
				return false;
			}
		}

		return true;
	}

	/*
	 * test if a collection contains a substring:
	 */
	template<typename LessThan, typename TGenomeBase, typename TIterator>
	static bool _substr_exists(const TGenomeBase& base,
	                           const LessThan& lt,
	                           const TIterator& start,
	                           const TIterator& end,
	                           const typename TGenomeBase::sequence_type& seq,
	                           const sequence_len_t offset0, const sequence_len_t offset1)
	{
		TIterator iter = start;

		while(iter != end)
		{
			if(_cmp_substr(base, lt, iter->source->at(iter->index), iter->start, iter->end, seq, offset0, offset1))
			{
				return true;
			}

			++iter;
		}

		return false;
	}

	/**
	   @tparam TGenomeBase genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @param source a population
	   @return the substring diversity

	   @brief Computes the substring diversity of a population.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	double substring_diversity(IInputAdapter<typename TGenomeBase::sequence_type>& source, LOGBase lbase = LOGBASE_E)
	{
		static TGenomeBase base;
		static LessThan lt;
		std::size_t i = 0;
		double d = 0.0;

		/* instead of copying genes we store the start & end offset of each found substring to identify it */
		struct _Range
		{
			IInputAdapter<typename TGenomeBase::sequence_type>* source;
			std::size_t index;
			sequence_len_t start;
			sequence_len_t end;
		} r;

		std::vector<struct _Range> substrs;     // substrings found in current sequence
		std::vector<struct _Range> substrs_all; // substrings found in all sequences

		// get all sequences:
		source.first();

		while(!source.end())
		{
			// get current sequence & its length:
			auto seq = source.current();
			auto len = base.len(seq);

			// clear vector to store substrings:
			substrs.clear();

			// find all substrings:
			for(sequence_len_t sublen = 0; sublen < len; ++sublen)
			{
				for(sequence_len_t start = 0; start < len - sublen; ++start)
				{
					r.source = &source;
					r.index = i;
					r.start = start;
					r.end = start + sublen;

					// has substring already been found?
					if(!_substr_exists<LessThan>(base, lt, begin(substrs), end(substrs), seq, start, start + sublen))
					{
						// no => add found substring to vector(s):
						substrs.push_back(r);

						if(!_substr_exists<LessThan>(base, lt, begin(substrs_all), end(substrs_all), seq, start, start + sublen))
						{
							substrs_all.push_back(r);
						}
					}
				}
			}

			d += substrs.size();

			source.next(), ++i;
		}

		d = (double)source.size() * substrs_all.size() / d;

		return d;
	}

	/**
	   @}
	 */
}
#endif
