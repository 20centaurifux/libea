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
   @file TournamentSelection.hpp
   @brief Compares the fitness of N random individuals to Q random of opponents.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_TOURNAMENT_SELECTION_HPP
#define EA_TOURNAMENT_SELECTION_HPP

#include <iterator>
#include <limits>
#include <numeric>
#include <vector>
#include <stdexcept>

#include "Random.hpp"
#include "Utils.hpp"
#include "Fitness.hpp"

namespace ea::selection
{
	/**
	   @addtogroup Selection
	   @{
	 */

	/**
	   @class Tournament
	   @brief Selects N random individuals from a population. Then the fitness of each
	          chromosome is compared to Q random opponents. The fittest individual is chosen.
	 */
	template<typename InputIterator, typename Compare = std::greater<double>>
	class Tournament
	{
		public:
			/**
			   @param Q number of random opponents each selected individual is compared to

			   Throws std::invalid_argument if Q is zero.
			 */
			explicit Tournament(const size_t Q = 3)
				: Q(Q)
			{
				if(Q == 0)
				{
					throw std::invalid_argument("Q cannot be zero.");
				}
			}

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

			   Throws std::length_error if \p Q exceeds the population size.
			 */
			template<typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t N, Fitness fitness, OutputIterator result)
			{
				length = std::distance(first, last);

				if(length < 0 || static_cast<typename std::make_unsigned<difference_type>::type>(length) <= Q)
				{
					throw std::length_error("Q exceeds population size.");
				}

				index_dist = std::uniform_int_distribution<difference_type>(0, length - 1);

				utils::repeat(N, [&]()
				{
					select(first, last, fitness, result);
				});
			}

		private:
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			const size_t Q;

			random::RandomEngine engine = random::default_engine();
			difference_type length;
			std::uniform_int_distribution<difference_type> index_dist;

			template<typename Fitness, typename OutputIterator>
			void select(InputIterator first, InputIterator last, Fitness fitness, OutputIterator result)
			{
				std::vector<difference_type> opponents(Q);

				random::fill_distinct_n_int(begin(opponents), Q, static_cast<difference_type>(0), length - 1);

				auto fitness_by_index = fitness::memoize_fitness_by_index<InputIterator>(fitness);
				difference_type index = random_index();

				while(std::find(begin(opponents), end(opponents), index) != end(opponents))
				{
					index = random_index();
				}

				*result++ = *(first + std::accumulate(begin(opponents), end(opponents), index, [&](auto index, auto i)
				{
					return compare_genotypes(first, index, i, fitness_by_index);
				}));
			}

			difference_type random_index()
			{
				return index_dist(engine);
			}

			template<typename Fitness>
			static difference_type compare_genotypes(InputIterator first, const difference_type a, const difference_type b, Fitness fitness)
			{
				difference_type index = b;

				if(Compare()(fitness(first, a), fitness(first, b)))
				{
					index = a;
				}

				return index;
			}
	};

	/*! @} */
}

#endif

