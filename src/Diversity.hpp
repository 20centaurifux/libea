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
   @file Diversity.hpp
   @brief Various algorithms to compute the diversity of a population.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef EA_DIVERSITY_HPP
#define EA_DIVERSITY_HPP

#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <tuple>
#include <numeric>
#include <cfenv>
#include <functional>
#include <type_traits>
#include <cmath>
#include <stdexcept>
#include <iostream>

#include "Utils.hpp"

namespace ea::diversity
{
	/**
	   @addtogroup Utils
	   @{
	 */

	/**
	   @tparam InputIterator must meet the requirements of LegacyInputIterator
	   @param first1 points to the first element of the first chromosome
	   @param last1 points to the end of the first chromosome
	   @param first2 points to the first element of the second chromosome
	   @param last2 points to the end of the second chromosome
	   @return the hamming distance

	   Calculates the hamming distance between two chromosomes.

	   Throws std::length_error if chromosome lengths differ and std::overflow_error if
	   the calculated distance value overflows.
	 */
	template<typename InputIterator>
	size_t hamming_distance(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2)
	{
		InputIterator it1 = first1;
		InputIterator it2 = first2;
		size_t d = 0;

		while(it1 != last1 && it2 != last2)
		{
			if(*it1 != *it2)
			{
				if(d == std::numeric_limits<size_t>::max())
				{
					throw std::overflow_error("Arithmetic overflow.");
				}

				++d;
			}

			++it1;
			++it2;
		}

		if(it1 != last1 || it2 != last2)
		{
			throw std::length_error("Set lengths have to be equal.");
		}

		return d;
	}

	/**
	   @tparam InputIterator must meet the requirements of LegacyInputIterator
	   @param first first individual of a population
	   @param last points to the past-the-end element in the sequence
	   @return average hamming distance of a population

	   Calculates the average hamming distance of a population.

	   Throws std::length_error if chromosome lengths differ and std::overflow_error if
	   the calulcated distance value overflows.

	   Throws std::length_error if chromosome lengths differ and std::overflow_error if
	   the calculated distance value overflows.
	 */
	template<typename InputIterator>
	double avg_hamming_distance(InputIterator first, InputIterator last)
	{
		const auto length = std::distance(first, last);
		double avg = 0.0;

		if(length > 0)
		{
		    size_t total = 0;

			for(InputIterator it1 = first; it1 != last; ++it1)
			{
				for(InputIterator it2 = it1 + 1; it2 != last; ++it2)
				{
					size_t d = hamming_distance(begin(*it1), end(*it1), begin(*it2), end(*it2));

					if(std::numeric_limits<size_t>::max() - total < d)
					{
						throw std::overflow_error("Arithmetic overflow.");
					}

					total += d;
				}
			}

			std::feclearexcept(FE_OVERFLOW);

			avg = static_cast<double>(total) / ((static_cast<double>(length - 1) * length) / 2);

			if(std::fetestexcept(FE_OVERFLOW))
			{
				throw std::overflow_error("Arithmetic overflow.");
			}
		}

		return avg;
	}

	/*! Signature of a logarithmic function. */
	using LogFn = double(*)(const double);

	/**
	   @tparam log a logarithmic function
	   @tparam InputIterator must meet the requirements of LegacyInputIterator
	   @param first points to the first element of the first chromosome
	   @param last points to the past-the-end element in the sequence
	   @return Shannon entropy

	   Calculates the Shannon entropy of a chromosome.

	   Throws std::overflow_error if the calculated entropy value overflows.
	 */
	template<LogFn log = std::log, typename InputIterator>
	double shannon_entropy(InputIterator first, InputIterator last)
	{
		const auto length = std::distance(first, last);
		double entropy = 0.0;

		if(length > 0)
		{
			using Gene = typename std::iterator_traits<InputIterator>::value_type;

			std::map<Gene, typename std::remove_const<decltype(length)>::type> map;

			std::for_each(first, last, [&map](const auto &g)
			{
				map[g]++;
			});

			std::feclearexcept(FE_UNDERFLOW);

			entropy = std::accumulate(begin(map), end(map), 0.0, [&length](double e, const auto &kv)
			{
				auto f = static_cast<double>(kv.second) / length;

				if(std::fpclassify(f) != FP_ZERO)
				{
					e -= f * log(f);

					if(std::fetestexcept(FE_UNDERFLOW))
					{
						throw std::underflow_error("Arithmetic underflow.");
					}
				}

				return e;
			});
		}

		return entropy;
	}

	/**
	   @tparam InputIterator must meet the requirements of LegacyInputIterator
	   @param first first individual of a population
	   @param last points to the past-the-end element in the sequence
	   @return average shannon entropy of a population

	   Calculates the average shannon entropy of a population.

	   Throws std::overflow_error if the calculated entropy value overflows.
	 */
	template<LogFn log = std::log, typename InputIterator>
	double avg_shannon_entropy(InputIterator first, InputIterator last)
	{
		const auto length = std::distance(first, last);
		double avg = 0.0;

		if(length > 0)
		{
			std::feclearexcept(FE_OVERFLOW);

			avg = std::accumulate(first, last, 0.0, [](const auto entropy, const auto chromosome)
			{
				auto total = entropy + shannon_entropy<log>(begin(chromosome), end(chromosome));

				if(std::fetestexcept(FE_OVERFLOW))
				{
					throw std::overflow_error("Arithmetic overflow.");
				}

				return total;
			}) / length;
		};

		return avg;
	}

	/**
	   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
	   @param first first individual of a population
	   @param last points to the past-the-end element in the sequence
	   @return substring diversity

	   Calculates the substring diversity of a population.

	   Throws std::overflow_error if the calculated diversity value overflows.
	 */
	template<typename InputIterator>
	double substr_diversity(InputIterator first, InputIterator last)
	{
		using Char = typename InputIterator::value_type::iterator::value_type;
		using Substr = std::vector<Char>;
		using Set = std::set<Substr>;

		Set all;

		const auto total = std::accumulate(first, last, 0, [&all](const typename Set::size_type total, auto &chromosome)
		{
			const auto length = std::distance(begin(chromosome), end(chromosome));
			Set substrs;

			for(typename std::remove_const<decltype(length)>::type sublen = 1; sublen < length; ++sublen)
			{
				Substr substr(sublen);

				for(typename std::remove_const<decltype(length)>::type start = 0; start <= length - sublen; ++start)
				{
					std::copy_n(begin(chromosome) + start, sublen, begin(substr));

					substrs.insert(substr);
					all.insert(substr);
				}
			}

			if(std::numeric_limits<typename Set::size_type>::max() - total < substrs.size())
			{
				throw std::overflow_error("Arithmetic overflow.");
			}

			return total + substrs.size();
		});

		std::feclearexcept(FE_OVERFLOW);

		const double diversity = static_cast<double>(std::distance(first, last) * all.size());

		if(std::fetestexcept(FE_OVERFLOW))
		{
		    throw std::overflow_error("Arithmetic overflow.");
		}

		return diversity / total;
	}

	/**
	   @}
	 */
}

#endif

