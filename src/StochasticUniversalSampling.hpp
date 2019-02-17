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
   @file StochasticUniversalSampling.hpp
   @brief The probability for being selected is proportional to the
          fitness value of an individual. A single random value to sample all
	  of the solutions is used.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_STOCHASTIC_UNIVERSAL_SAMPLING_HPP
#define EA_STOCHASTIC_UNIVERSAL_SAMPLING_HPP

#include <iterator>
#include <stdexcept>
#include <vector>
#include <complex>
#include <numeric>
#include <limits>
#include <algorithm>
#include <cfenv>

#include "Random.hpp"
#include "Utils.hpp"

namespace ea::selection
{
	/**
	   @addtogroup Selection
	   @{
	 */

	/**
	   @class StochasticUniversalSampling

	   Selects N individuals from a population. The probability for being selected
	   is proportional to the fitness value of an individual.
	 */
	template<typename InputIterator>
	class StochasticUniversalSampling
	{
		public:
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
				std::vector<double> sums;

				const double min = collect(first, last, fitness, std::back_inserter(sums));

				if(N > 0 && sums.size() == 0)
				{
					throw std::length_error("Population is empty.");
				}

				const double total = accumulate(begin(sums), end(sums), min);

				random::RandomEngine eng = random::default_engine();
				std::uniform_real_distribution<double> dist(0.0, total / N);

				double u = dist(eng);
				auto upper = begin(sums);

				utils::repeat(N, [&]()
				{
					upper = std::upper_bound(begin(sums), end(sums), u);

					const auto offset = std::distance(begin(sums), upper);

					*result++ = *(first + offset);

					u += total / N;
				});
			}

		private:
			template<typename Fitness, typename OutputIterator>
			static double collect(InputIterator first, InputIterator last, Fitness fitness, OutputIterator result)
			{
				return std::accumulate(first, last, 0.0, [&fitness, &result](const double min, auto &chromosome)
				{
					double f = fitness(begin(chromosome), end(chromosome));

					*result++ = f;

					return std::min(f, min);
				});
			}

			template<typename Iterator>
			static double accumulate(Iterator first, Iterator last, const double min)
			{
				const double alignment = std::abs(std::min(0.0, min)) * 2;

				if(std::fetestexcept(FE_OVERFLOW))
				{
					throw std::overflow_error("Arithmetic overflow.");
				}

				double sum = 0.0;

				while(first != last)
				{
					sum += *first + alignment;

					if(std::fetestexcept(FE_OVERFLOW))
					{
						throw std::overflow_error("Arithmetic overflow.");
					}

					*first++ = sum;
				}

				return sum;
			}
	};

	/*! @} */
}

#endif

