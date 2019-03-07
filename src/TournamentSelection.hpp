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
	   @class Tournament
	   @tparam Compare function to compare fitness values
	   @brief Selects N random individuals from a population. Then the fitness of each
	          chromosome is compared to Q random opponents. The fittest individual is chosen.
	 */
	template<typename Compare = std::greater<double>>
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
			   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
			   @tparam Fitness fitness function object: double fun(InputIterator first, InputIterator last)
			   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
			   @param first first individual of a population
			   @param last points to the past-the-end element in the sequence
			   @param fitness a fitness function
			   @param N number of individuals to select from the population
			   @param result beginning of the destination range

			   Selects \p N individuals from a population and copies them to \p result.

			   Throws std::length_error if \p Q exceeds the population size.
			 */
			template<typename InputIterator, typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t N, Fitness fitness, OutputIterator result) const
			{
				const difference_type<InputIterator> length = std::distance(first, last);

				if(length < 0 || static_cast<typename std::make_unsigned<difference_type<InputIterator>>::type>(length) <= Q)
				{
					throw std::length_error("Q exceeds population size.");
				}

				random::RandomEngine engine = random::default_engine();
				std::uniform_int_distribution<difference_type<InputIterator>> index_dist(0, length - 1);

				utils::repeat(N, [&]()
				{
					std::vector<difference_type<InputIterator>> opponents(Q);

					random::fill_distinct_n_int(begin(opponents), Q, static_cast<difference_type<InputIterator>>(0), length - 1);

					auto fitness_by_index = fitness::memoize_fitness_by_index<InputIterator>(fitness);
					difference_type<InputIterator> index = index_dist(engine);

					while(std::find(begin(opponents), end(opponents), index) != end(opponents))
					{
						index = index_dist(engine);
					}

					*result++ = *(first + std::accumulate(begin(opponents), end(opponents), index, [&](auto index, auto i)
					{
						return compare_genotypes(first, index, i, fitness_by_index);
					}));
				});
			}

		private:
			template<typename InputIterator>
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			const size_t Q;

			template<typename InputIterator, typename Fitness>
			static difference_type<InputIterator> compare_genotypes(InputIterator first,
			                                                        const difference_type<InputIterator> a,
			                                                        const difference_type<InputIterator> b,
			                                                        Fitness fitness)
			{
				difference_type<InputIterator> index = b;

				if(Compare()(fitness(first, a), fitness(first, b)))
				{
					index = a;
				}

				return index;
			}
	};
}

#endif

