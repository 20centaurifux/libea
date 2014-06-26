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
   @file algo.h
   @brief Some generic algorithms.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ALGO_H
#define ALGO_H

#include <cassert>
#include <limits>
#include <vector>
#include <set>
#include <algorithm>

namespace ea
{
	/**
	   @tparam TContainer type of the genome container
	   @param pop a collection containing genomes
	   @return a double
	   
	   Gets arithmetic average of a population.
	 */
	template<typename TContainer>
	double mean(const TContainer& pop)
	{
		double sum = 0;
		uint32_t count = 0;

		for(auto g : pop)
		{
			sum += g->fitness();

			assert(count != UINT32_MAX);
			count++;
		}

		return sum / count;
	}

	/**
	   @tparam TContainer type of the genome container
	   @param pop a collection containing genomes
	   @return a double
	   
	   Gets the higher half of a population.
	 */
	template<typename TContainer>
	double median(const TContainer& pop)
	{
		std::vector<double> values;

		for(auto g : pop)
		{
			values.push_back(g->fitness());
		}

		std::sort(begin(values), end(values));

		return values[values.size() / 2];
	}

	/**
	   @tparam TGenome type of the genome class
	   @param a a genome
	   @param b a genome
	   @return true if both genomes are equal
	   
	   Tests if two genomes contain exactly the same genes.
	 */
	template<typename TGenome>
	bool equals(const TGenome& a, const TGenome& b)
	{
		if(a.size() != b.size())
		{
			return false;
		}

		std::multiset<typename TGenome::value_type> s0;
		std::multiset<typename TGenome::value_type> s1;

		for(uint32_t i = 0; i < a.size(); i++)
		{
			s0.insert(a.at(i));
			s1.insert(b.at(i));
		}

		return s0 == s1;
	}

	/**
	   @tparam TGenome type of the genome class
	   @param a pointer to a genome
	   @param b pointer to a genome
	   @return true if both genomes are equal
	   
	   Tests if two genomes contain exactly the same genes. The corresponding gene class
	   must implement a less_than() method.
	 */
	template<typename TGenome>
	bool equals(const TGenome* a, const TGenome* b)
	{
		typedef struct
		{
			bool operator()(typename TGenome::value_type a, typename TGenome::value_type b)
			{
				return a->less_than(b);
			}
		} LessThan;

		if(a->size() != b->size())
		{
			return false;
		}

		std::multiset<typename TGenome::value_type, LessThan> s0;
		std::multiset<typename TGenome::value_type, LessThan> s1;

		for(uint32_t i = 0; i < a->size(); i++)
		{
			s0.insert(a->at(i));
			s1.insert(b->at(i));
		}

		return s0 == s1;
	}
}
#endif

