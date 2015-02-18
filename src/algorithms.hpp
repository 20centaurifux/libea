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
   @file algorithms.hpp
   @brief Various algorithms.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <math.h>
#include <assert.h>
#include <algorithm>
#include <set>
#include <limits>
#include "AGenome.hpp"
#include "InputAdapter.hpp"

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

		for(sequence_len_t i = 0; i < base.len(seq); ++i)
		{
			const typename TGenomeBase::gene_type& gene = base.get(seq, i);

			if(set.find(gene) != end(set))
			{
				return true;
			}

			set.insert(gene);
		}

		return false;
	}

	/**
	   @tparam TGenomeBase a genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @return true if both sequences are equal.

	   Tests if two sequences are qual. The sequences have to be sets.
	  */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	bool set_equals(const typename TGenomeBase::sequence_type& a, const typename TGenomeBase::sequence_type& b)
	{
		assert(!is_multiset<TGenomeBase>(a));
		assert(!is_multiset<TGenomeBase>(b));

		static TGenomeBase base;
		sequence_len_t len = base.len(a);

		if(len != base.len(b))
		{
			return false;
		}

		for(sequence_len_t i = 0; i < len; ++i)
		{
			if(base.index_of(a, base.get(b, i)) == -1)
			{
				return false;
			}
		}

		return true;
	}

	/*! Tests if a floating point number is normal (not NAN, subnormal or infinite). */
	#define ASSERT_FP_NORMALITY(f) assert(std::fpclassify(f) == FP_NORMAL || std::fpclassify(f) == FP_ZERO)

	/*! Macro to test if an addition overflows. */
	#define ASSERT_ADDITION(T, a, b) \
		assert(!((b > 0 && a > (std::numeric_limits<T>::max() - b)) || \
		         (b < 0 && a < (std::numeric_limits<T>::min() - b))))

	/**
	   \tparam T type of terms to test
	   \param a first term
	   \param b second term
	   
	   Tests if an addition overflows.
	  */
	template<typename T>
	inline void assert_addition(const T a, const T b)
	{
		ASSERT_ADDITION(T, a, b);
	}

	/**
	   \param a first term
	   \param b second term
	   
	   Tests if two doubles can be added without overflow.
	  */
	template<>
	inline void assert_addition(const double a, const double b)
	{
		ASSERT_FP_NORMALITY(a);
		ASSERT_FP_NORMALITY(b);
		ASSERT_ADDITION(double, a, b);
	}

	/**
	   \tparam T type of the terms to add
	   \param a first term
	   \param b second term
	   \return the sum of a and b

	   Performs overflow check and adds to values.
	  */
	template<typename T>
	inline T chk_add(const T a, const T b)
	{
		assert_addition(a, b);

		return a + b;
	}

	/**
	   \tparam T type of values to add
	   \param list list of values to add
	   \return sum of all values from an initializer_list

	   Adds values from the initializer_list and performs overflow check.
	  */
	template<typename T>
	T chk_add(std::initializer_list<T> list)
	{
		auto iter = std::begin(list);
		T sum = *iter;
		
		++iter;

		while(iter != end(list))
		{
			ASSERT_ADDITION(T, sum, *iter);
			sum += *iter;
			++iter;
		}

		return sum;
	}

	/*! Macro to test if a substraction overflows. */
	#define ASSERT_SUBTRACTION(T, a, b) \
		assert(!((b > 0 && a < (std::numeric_limits<T>::min() + b)) || \
		         (b < 0 && a > (std::numeric_limits<T>::max() + b))));

	/**
	   \tparam T type of terms to test
	   \param a minuend
	   \param b subtrahend
	   
	   Tests if a subtraction can overflow.
	  */
	template<typename T>
	inline void assert_subtraction(const T a, const T b)
	{
		ASSERT_SUBTRACTION(T, a, b);
	}

	/**
	   \tparam T type of terms to test
	   \param a minuend
	   \param b subtrahend
	   
	   Tests if a subtraction of two doubles overflows.
	  */
	template<>
	inline void assert_subtraction(const double a, const double b)
	{
		ASSERT_FP_NORMALITY(a);
		ASSERT_FP_NORMALITY(b);
		ASSERT_SUBTRACTION(double, a, b);
	}

	/**
	   \tparam T type of the terms to subtract
	   \param a minuend
	   \param b subtrahend
	   \return total difference

	   Performs overflow check and subtracts b from a. 
	  */
	template<typename T>
	inline T chk_sub(const T a, const T b)
	{
		assert_subtraction(a, b);

		return a - b;
	}

	/**
	   \tparam T type of values to subtract
	   \param list list of values to subtract
	   \return total difference

	   Subtracts rest of the the initializer_list from the first value and
	   performs overflow check.
	  */
	template<typename T>
	T chk_sub(std::initializer_list<T> list)
	{
		auto iter = std::begin(list);
		T diff = *iter;
		
		++iter;

		while(iter != end(list))
		{
			ASSERT_SUBTRACTION(T, diff, *iter);
			diff -= *iter;
			++iter;
		}

		return diff;
	}

	/**
	 * @tparam TGenomeBase a genome base class
	 * @param source source of sequences
	 * @return average fitness
	 *
	 * Calculates the average fitness of a population.
	 **/
	template<typename TGenomeBase>
	double mean(IInputAdapter<typename TGenomeBase::sequence_type>& source)
	{
		TGenomeBase base;
		double result = 0;

		source.first();

		for(size_t i = 0; i < source.size(); ++i)
		{
			result += base.fitness(source.at(i));
		}

		return result / source.size();
	}

	/**
	 * @tparam TGenomeBase a genome base class
	 * @param source source of sequences
	 * @return median fitness
	 *
	 * Calculates the median fitness of a population.
	 **/
	template<typename TGenomeBase>
	double median(IInputAdapter<typename TGenomeBase::sequence_type>& source)
	{
		TGenomeBase base;
		std::vector<double> values;

		source.first();

		for(size_t i = 0; i < source.size(); ++i)
		{
			values.push_back(base.fitness(source.at(i)));
		}

		std::sort(begin(values), end(values));

		return values[(end(values) - begin(values)) / 2];
	}

	/**
	   @tparam TGenomeBase genome base class
	   @tparam Compare a compare function
	   @param source a population
	   @return index of the fittest genome

	   @brief Finds the fittest genome of a population.
	 */
	template<typename TGenomeBase, typename Compare = std::greater<double>>
	size_t fittest(IInputAdapter<typename TGenomeBase::sequence_type>& source)
	{
		TGenomeBase base;
		Compare cmp;
		size_t index = 0;

		source.first();

		for(auto i = 1; i < source.size(); ++i)
		{
			if(cmp(base.fitness(source.at(index)), base.fitness(source.at(i))) < 1)
			{
				index = i;
			}
		};

		return index;
	}

	/**
	   @}
	 */
}
#endif
