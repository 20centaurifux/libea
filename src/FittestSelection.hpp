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
   @file FittestSelection.hpp
   @brief Selects the N fittest individuals.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_FITTEST_SELECTION_HPP
#define EA_FITTEST_SELECTION_HPP

#include <limits>
#include <set>
#include <stdexcept>

namespace ea::selection
{
	/**
	   @class Fittest
	   @tparam Compare function to compare fitness values
	   @brief Selects the N fittest individuals.
	 */
	template<typename Compare = std::greater<double>>
	class Fittest
	{
		public:
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

			   Throws std::length_error if N exceeds population size or std::overflow_error
			   if the number of individuals exceeds size_t.
			 */
			template<typename InputIterator, typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t N, Fitness fitness, OutputIterator result) const
			{
				std::multiset<Chromosome> chromosomes;
				size_t i = 0;

				for(auto p = first; p != last; ++p, ++i)
				{
					chromosomes.insert({ i, fitness(begin(*p), end(*p)) });

					if(i == std::numeric_limits<size_t>::max())
					{
						throw std::overflow_error("Arithmetic overflow.");
					}
				};

				if(chromosomes.size() < N)
				{
					throw std::length_error("N exceeds population size.");
				}

				i = 0;

				for(auto c = begin(chromosomes); c != end(chromosomes) && i < N; ++c, ++i, ++result)
				{
					*result = *(first + c->index);
				};
			}

		private:
			struct Chromosome
			{
				const size_t index;
				const double fitness;

				bool operator<(const struct Chromosome& rhs) const
				{
					return Compare()(fitness, rhs.fitness);
				}
			};
	};
}

#endif

