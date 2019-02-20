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
   @file DoubleTournamentSelection.hpp
   @brief Compares the fitness of all individuals to Q random opponents.
          The chromosomes with the most victories are selected.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_DOUBLE_TOURNAMENT_SELECTION_HPP
#define EA_DOUBLE_TOURNAMENT_SELECTION_HPP

#include <iterator>
#include <limits>
#include <vector>
#include <algorithm>
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
	   @class DoubleTournament
	   @tparam Compare function to compare fitness values
	   @brief Compares the fitness of all individuals to Q random opponents.
                  The chromosomes with the most victories are selected.
	 */
	template<typename Compare = std::greater<double>>
	class DoubleTournament
	{
		public:
			/**
			   @param Q number of random opponents each individual is compared to

			   Throws std::invalid_argument if Q is zero.
			 */
			explicit DoubleTournament(const size_t Q = 10)
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

			   Throws std::length_error if \p Q or \p N exceeds the population size.
			 */
			template<typename InputIterator, typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t N, Fitness fitness, OutputIterator result)
			{
				const auto length = std::distance(first, last);

				if(length < 0 || static_cast<typename std::make_unsigned<difference_type<InputIterator>>::type>(length) <= Q)
				{
					throw std::length_error("Q exceeds population size.");
				}

				if(N > 0 && N >= static_cast<typename std::make_unsigned<difference_type<InputIterator>>::type>(length))
				{
					throw std::length_error("N exceeds population size.");
				}

				auto fitness_by_index = fitness::memoize_fitness_by_index<InputIterator>(fitness);

				std::vector<Score<InputIterator>> indeces;
				difference_type<InputIterator> i = 0;

				for(auto chromosome = first; chromosome != last; ++chromosome, ++i)
				{
					Score<InputIterator> score = { i, 0 };

					std::vector<difference_type<InputIterator>> opponents(Q);

					random::fill_distinct_n_int(begin(opponents), Q, static_cast<difference_type<InputIterator>>(0), length - 1);

					std::for_each(begin(opponents), end(opponents), [&](difference_type<InputIterator> j)
					{
						if(Compare()(fitness_by_index(first, i), fitness_by_index(first, j)))
						{
							++score.value;
						}
					});

					indeces.push_back(score);
				}

				std::sort(begin(indeces), end(indeces));

				std::transform(begin(indeces), begin(indeces) + N, result, [&](auto &score)
				{
					return *(first + score.offset);
				});
			}

		private:
			template<typename InputIterator>
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			template<typename InputIterator>
			struct Score
			{
				difference_type<InputIterator> offset;
				size_t value;

				bool operator<(const struct Score& rhs) const
				{
					return value > rhs.value;
				}
			};

			const size_t Q;
	};

	/*! @} */
}

#endif

