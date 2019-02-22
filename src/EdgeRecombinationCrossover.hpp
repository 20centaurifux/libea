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
   @file EdgeRecombinationCrossover.hpp
   @brief Creates a path that is similar to a set of existing paths by looking at
          the edges rather than the vertices.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
*/
#ifndef EA_EDGE_RECOMBINATION_CROSSOVER_HPP
#define EA_EDGE_RECOMBINATION_CROSSOVER_HPP

#include <iterator>
#include <set>
#include <map>
#include <algorithm>
#include <stdexcept>

#include "Random.hpp"
#include "Utils.hpp"

namespace ea::crossover
{
	/**
	   @addtogroup Crossover
	   @{
	*/

	/**
	   @class EdgeRecombination
	   @tparam Chromosome must meet the requirements of LegacyRandomAccessIterator
	   @brief Creates a path that is similar to a set of existing paths by looking at
	          the edges rather than the vertices.
	*/
	template<typename Chromosome>
	class EdgeRecombination
	{
		public:
			/**
			   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
			   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
			   @param first1 points to the first element of the first chromosome
			   @param last1 points to the end of the first chromosome
			   @param first2 points to the first element of the second chromosome
			   @param last2 points to the end of the second chromosome
			   @param result beginning of the destination range
			   @return number of offsprings written to \p result

			   Combines two parents and generates a single offspring.

			   Throws std::length_error or std::logic_error if the genes of the parent
			   chromosomes aren't the same.
			*/
			template<typename InputIterator, typename OutputIterator>
			size_t operator()(InputIterator first1,
					  InputIterator last1,
					  InputIterator first2,
					  InputIterator last2,
					  OutputIterator result)
			{
				const difference_type<InputIterator> length = std::distance(first1, last1);

				if(length != std::distance(first2, last2))
				{
					throw std::length_error("Chromosome lengths have to be equal.");
				}

				std::uniform_int_distribution<difference_type<InputIterator>> dist(0, length - 1);
				difference_type<InputIterator> offset = dist(eng);

				NeighborMap map = build_map(first1, last1, first2, last2);
				std::vector<Gene> offspring;

				while(offspring.size() != static_cast<typename std::make_unsigned<difference_type<InputIterator>>::type>(length))
				{
					Gene neighbor = *(first1 + offset);

					offspring.push_back(neighbor);

					remove_neighbor<InputIterator>(map, neighbor);

					GeneSequence neighbors = map[neighbor];

					if(neighbors.size() == 0)
					{
						do
						{
							offset = dist(eng);
						} while(std::find(begin(offspring), end(offspring), *(first1 + offset)) == end(offspring));
					}
					else
					{
						typename GeneSequence::iterator it = best_neighbor(map, begin(neighbors), end(neighbors));
						InputIterator match = std::find(first1, last1, *it);

						offset = std::distance(first1, match);
					}
				}

				Chromosome chromosome(length);

				std::move(begin(offspring), end(offspring), begin(chromosome));

				*result++ = chromosome;

				return 1;
			}

		private:
			using Gene = typename Chromosome::value_type;
			using GeneSequence = typename std::set<Gene>;
			using NeighborMap = typename std::map<Gene, GeneSequence>;

			template<typename InputIterator>
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			random::RandomEngine eng = random::default_engine();

			template<typename InputIterator>
			NeighborMap build_map(InputIterator first1,
			                     InputIterator last1,
			                     InputIterator first2,
			                     InputIterator last2)
			{
				NeighborMap map;

				for(InputIterator pos1 = first1; pos1 != last1; ++pos1)
				{
					GeneSequence neighbors;

					neighbors.insert(*utils::prev(pos1, first1, last1));
					neighbors.insert(*utils::next(pos1, first1, last1));

					InputIterator pos2 = std::find(first2, last2, *pos1);

					if(pos2 == last2)
					{
						throw std::logic_error("Chromosomes aren't equal.");
					}

					neighbors.insert(*utils::prev(pos2, first2, last2));
					neighbors.insert(*utils::next(pos2, first2, last2));

					map[*pos1] = neighbors;
				}

				return map;
			}

			template<typename InputIterator>
			static void remove_neighbor(NeighborMap &map, const Gene &neighbor)
			{
				std::for_each(begin(map), end(map), [&neighbor](auto &kv)
				{
					kv.second.erase(neighbor);
				});
			}

			template<typename InputIterator>
			InputIterator best_neighbor(NeighborMap map, InputIterator first, InputIterator last)
			{
				std::vector<InputIterator> shuffled;

				for(InputIterator it = first; it != last; ++it)
				{
					shuffled.push_back(it);
				};

				std::shuffle(begin(shuffled), end(shuffled), eng);

				return *std::min_element(begin(shuffled), end(shuffled), [&](InputIterator &a, InputIterator &b)
				{
					return map[*a].size() < map[*b].size();
				});
			}
	};

	/*! @} */
}

#endif

