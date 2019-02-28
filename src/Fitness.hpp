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
   @file Fitness.hpp
   @brief Fitness related utility functions.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_FITNESS_HPP
#define EA_FITNESS_HPP

#include <numeric>
#include <functional>
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <limits>
#include <cfenv>
#include <stdexcept>

namespace ea::fitness
{
	/**
	   @addtogroup Utils
	   @{
	 */

	/**
	   @tparam PopulationIterator iterator type pointing to a chromosome

	   Use this template to get the chromosome's iterator type.
	 */
	template<typename PopulationIterator>
	using ChromosomeIterator = typename std::iterator_traits<PopulationIterator>::value_type::iterator;

	/**
	   @tparam PopulationIterator iterator type pointing to a chromosome

	   Fitness function object.
	 */
	template<typename PopulationIterator>
	using FitnessFunction = std::function<double(ChromosomeIterator<PopulationIterator>,
	                                             ChromosomeIterator<PopulationIterator>)>;

	/**
	   @tparam PopulationIterator must meet the requirements of LegacyRandomAccessIterator
	   @param fn a fitness function
	   @return a new function object

	   Wraps a fitness function & returns a new function object to get the fitness
	   of a chromosome by index.
	 */	
	template<typename PopulationIterator>
	std::function<double(PopulationIterator, const size_t)>
	fitness_by_index(FitnessFunction<PopulationIterator> fn)
	{
		return [fn](PopulationIterator first, const size_t index)
		{
			auto &chromosome = *(first + index);

			return fn(std::begin(chromosome), std::end(chromosome));
		};
	}

	/**
	   @class MemoizeFitnessByIndex
	   @tparam PopulationIterator must meet the requirements of LegacyRandomAccessIterator
	   @brief A functor to get the fitness of a chromosome by index. The fitness value
	          is cached for each index.
	 */
	template<typename PopulationIterator>
	class MemoizeFitnessByIndex
	{
		public:
			/**
			   @param fn a fitness functions

			   Creates the functor and wraps the given fitness function.
			 */
			explicit MemoizeFitnessByIndex(FitnessFunction<PopulationIterator> fn)
				: fn(fitness_by_index<PopulationIterator>(fn))
			{}

			/**
			   @param population iterator pointing to the first chromosome of a population
			   @param index index of the desired chromosome
			   @return fitness of the chromosome at the specifed position

			   Returns and caches the fitness of a chromosome at the given position.
			 */
			double operator()(PopulationIterator population, const size_t index)
			{
				double fitness;
				auto found = cache.find(index);

				if(found == end(cache))
				{
					fitness = fn(population, index);
					cache[index] = fitness;
				}
				else
				{
					fitness = found->second;
				}

				return fitness;
			}

		private:
			const std::function<double(PopulationIterator, const size_t)> fn;
			std::map<size_t, double> cache;
	};

	/**
	   @tparam PopulationIterator must meet the requirements of LegacyRandomAccessIterator
	   @param fn a fitness function
	   @return a new function object

	   Wraps a fitness function & returns a new function object to get the fitness
	   of a chromosome by index. The fitness is cached for each index.
	 */	
	template<typename PopulationIterator>
	std::function<double(PopulationIterator, const size_t)>
	memoize_fitness_by_index(FitnessFunction<PopulationIterator> fn)
	{
		return MemoizeFitnessByIndex<PopulationIterator>(fn);
	}

	/**
	   @tparam PopulationIterator must meet the requirements of LegacyInputIterator
	   @param first iterator pointing to the first chromosome of a population
	   @param last iterator pointing to the end (element after the last element) of a population
	   @param fn a fitness function
	   @return mean fitness

	   Calculates the mean fitness value of a population by applying the given fitness
	   function \p fn to all chromosomes of the range \p first to \p last.

	   Throws std::overflow_error if the total fitness exceeds the supported maximum.
	 */	
	template<typename PopulationIterator>
	double mean(PopulationIterator first, PopulationIterator last, FitnessFunction<PopulationIterator> fn)
	{
		const auto distance = std::distance(first, last);

		std::feclearexcept(FE_OVERFLOW);

		const double sum = std::accumulate(first, last, 0.0, [&fn](double sum, auto chromosome)
		{
			sum += fn(begin(chromosome), end(chromosome));

			if(std::fetestexcept(FE_OVERFLOW))
			{
				throw std::overflow_error("Arithmetic overflow.");
			}

			return sum;
		});

		return distance > 0 ? sum / distance
		                    : std::numeric_limits<double>::quiet_NaN();
	}

	/**
	   @tparam PopulationIterator must meet the requirements of LegacyInputIterator
	   @param first iterator pointing to the first chromosome of a population
	   @param last iterator pointing to the end (element after the last element) of a population
	   @param fn a fitness function
	   @return mean fitness

	   Calculates the median fitness value of a population by applying the given fitness
	   function \p fn to all chromosomes of the range \p first to \p last.
	 */	
	template<typename PopulationIterator>
	double median(PopulationIterator first, PopulationIterator last, FitnessFunction<PopulationIterator> fn)
	{
		std::vector<double> values;

		std::for_each(first, last, [&values, &fn](auto chromosome)
		{
			values.push_back(fn(begin(chromosome), end(chromosome)));
		});

		std::sort(begin(values), end(values));

		return values.size() > 0 ? values[(end(values) - begin(values)) / 2]
		                         : std::numeric_limits<double>::quiet_NaN();
	}

	/**
	   @tparam PopulationIterator must meet the requirements of LegacyInputIterator
	   @tparam Compare function to compare fitness values
	   @param first iterator pointing to the first chromosome of a population
	   @param last iterator pointing to the end (element after the last element) of a population
	   @param fn a fitness function
	   @return iterator to the fittest chromosome

	   Finds the fittest chromosome by applying the function \p fn to all chromosomes
	   of the range \p first to \p last.
	 */	
	template<typename PopulationIterator, typename Compare = std::greater<double>>
	PopulationIterator fittest(PopulationIterator first, PopulationIterator last, FitnessFunction<PopulationIterator> fn)
	{
		auto fittest = first;
		double fitness = 0.0;

		if(fittest != last)
		{
			fitness = fn(begin(*fittest), end(*fittest));

			auto opponent = first;

			while(++opponent != last)
			{
				double new_fitness = fn(begin(*opponent), end(*opponent));

				if(Compare()(new_fitness, fitness))
				{
					fittest = opponent;
					fitness = new_fitness;
				}
			}
		}

		return fittest;
	}

	/*! @} */
}

#endif

