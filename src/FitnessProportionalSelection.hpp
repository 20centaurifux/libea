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
   @file FitnessProportionalSelection.hpp
   @brief The probability for being selected is proportional to the
          fitness value of an individual. Several chromosomes from the population
	  are selected by repeated random sampling.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_FITNESS_PROPORTIONAL_SELECTION_HPP
#define EA_FITNESS_PROPORTIONAL_SELECTION_HPP

#include <iterator>
#include <stdexcept>
#include <vector>
#include <complex>
#include <numeric>
#include <limits>
#include <algorithm>
#include <cfenv>

#include "Random.hpp"

namespace ea::selection
{
	/**
	   @addtogroup Selection
	   @{
	 */

	/**
	   @enum Proportionality
	   @brief Proportionality type.
	 */
	enum class Proportionality
	{
		/*! Probability is directly proportional to fitness value. */
		direct,
		/*! Probability is inversely proportional to fitness value. */
		inverse
	};

	/**
	   @class FitnessProportional

	   Selects N individuals from a population. The probability for being selected
	   is proportional to the fitness value of an individual.
	 */
	template<typename InputIterator>
	class FitnessProportional
	{
		public:
			/**
			   @param proportionality specifies if probability is directly or inversely
			                          proportional to the fitness value

			   Initializes the functor.
			 */
			FitnessProportional(Proportionality proportionality = Proportionality::direct)
				: proportionality(proportionality)
			{}

			/**
			   @tparam Fitness fitness function object: double fun(InputIterator first, InputIterator last)
			   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
			   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
			   @param first first individual of a population
			   @param last points to the past-the-end element in the sequence
			   @param fitness a fitness function
			   @param N number of individuals to select from the population
			   @param result beginning of the destination range

			   Selects \p N individuals from a population and copies them to \p result.

			   Throws std::length_error if population is empty or std::overflow_error if an
			   overflow occurs.
			 */
			template<typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t N, Fitness fitness, OutputIterator result)
			{
				std::vector<Slice> wheel;

				const size_t length = insert_slices(first, last, fitness, std::back_inserter(wheel));

				if(N > 0 && length == 0)
				{
					throw std::length_error("Population is empty.");
				}

				std::sort(begin(wheel), end(wheel));

				const double sum = align(begin(wheel), end(wheel));

				if(sum == 0) // all fitness values are zero
				{
					std::fill_n(result, N, *first);
				}
				else
				{
					accumulate(begin(wheel), end(wheel), sum);

					std::vector<double> numbers;

					random::fill_n_real(std::back_inserter(numbers), N, 0.0, 1.0);

					std::for_each(begin(numbers), end(numbers), [&](const double n)
					{
						auto slice = std::upper_bound(begin(wheel), end(wheel), n, [](const double n, const Slice &slice)
						{
							return n < slice.size;
						});

						if(proportionality == Proportionality::inverse)
						{
							auto offset = std::distance(slice, std::prev(end(wheel)));

							slice = begin(wheel) + offset;
						}

						*result = *(first + slice->index);
						++result;
					});
				}
			}

		private:
			typedef struct _Slice
			{
				size_t index;
				double size;

				bool operator<(const struct _Slice& rhs) const
				{
					return size < rhs.size;
				}
			} Slice;

			const Proportionality proportionality;

			template<typename Fitness, typename OutputIterator>
			static size_t insert_slices(InputIterator first, InputIterator last, Fitness fitness, OutputIterator result)
			{
				return std::accumulate(first, last, 0, [&fitness, &result](const size_t index, auto &chromosome)
				{
					if(index == std::numeric_limits<size_t>::max())
					{
						throw std::overflow_error("Arithmetic overflow.");
					}

					*result++ = { index, fitness(begin(chromosome), end(chromosome)) };

					return index + 1;
				});
			}

			template<typename Iterator>
			static double align(Iterator first, Iterator last)
			{
				std::feclearexcept(FE_OVERFLOW);

				const double alignment = std::abs(std::min(0.0, first->size)) * 2;

				test_fe_overflow();

				return std::accumulate(first, last, 0.0, [&alignment](double sum, Slice &slice)
				{
					slice.size += alignment;
					test_fe_overflow();

					sum += slice.size;
					test_fe_overflow();

					return sum;
				});
			}

			template<typename Iterator>
			static void accumulate(Iterator first, Iterator last, const double sum)
			{
				std::accumulate(first, last, 0.0, [sum](const double total, Slice &slice)
				{
					slice.size /= sum;
					slice.size += total;

					return slice.size;
				});
			}

			static void test_fe_overflow()
			{
				if(std::fetestexcept(FE_OVERFLOW))
				{
					throw std::overflow_error("Arithmetic overflow.");
				}
			}
	};

	/*! @} */
}

#endif

