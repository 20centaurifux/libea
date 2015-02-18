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
   @file TR1UniformDistribution.hpp
   @brief Random number generator using C++11 uniform distribution.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef TR1UNIFORMDISTRIBUTION_H
#define TR1UNIFORMDISTRIBUTION_H

#include <random>
#include <ctime>
#include <map>
#include <string>
#include <sstream>
#include <cstring>
#include "ARandomNumberGenerator.hpp"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	/**
	   @class TR1UniformDistribution
	   @tparam TEngine a random number engine
	   @brief Random number generator using C++11 uniform distribution.
	  */
	template<typename TEngine = std::mt19937_64>
	class TR1UniformDistribution : public ARandomNumberGenerator
	{
		public:
			TR1UniformDistribution()
			{
				_engine.seed(time(nullptr));

				memset(&_last_int32_dist, 0, sizeof(_last_int32_dist));
				memset(&_last_double_dist, 0, sizeof(_last_double_dist));
			}

			/**
			   @param r a random number generator to copy
			   @brief Copy constructor.
			  */
			TR1UniformDistribution(TR1UniformDistribution& r)
			{
				_engine.seed(time(nullptr));

				memset(&_last_int32_dist, 0, sizeof(_last_int32_dist));
				memset(&_last_double_dist, 0, sizeof(_last_double_dist));
			}

			virtual ~TR1UniformDistribution()
			{
				destroy_cache(_int32_cache);
				destroy_cache(_double_cache);
			}

			int32_t get_int32() override
			{
				return _int32_distribution(_engine);
			}

			double get_double() override
			{
				return _double_distribution(_engine);
			}

			int32_t get_int32(const int32_t min, const int32_t max) override
			{
				return (*get_distribution<std::uniform_int_distribution<int32_t>>
						(min, max, _int32_cache, _last_int32_dist))(_engine);
			}

			double get_double(const double min, const double max) override
			{
				return (*get_distribution<std::uniform_real_distribution<double>>
						(min, max, _double_cache, _last_double_dist))(_engine);
			}

			void get_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const std::size_t length) override
			{
				generate_sequence(get_distribution<std::uniform_int_distribution<int32_t>>
						(min, max, _int32_cache, _last_int32_dist), numbers, length);
			}

			void get_double_seq(const double min, const double max, double* numbers, const std::size_t length) override
			{
				generate_sequence(get_distribution<std::uniform_real_distribution<double>>
						(min, max, _double_cache, _last_double_dist), numbers, length);
			}

			inline int32_t get_max_int32() const override
			{
				return _int32_distribution.max();
			}

			inline int32_t get_min_int32() const override
			{
				return _int32_distribution.min();
			}

			double get_max_double() const override
			{
				return _double_distribution.max();
			}

			double get_min_double() const override
			{
				return _double_distribution.min();
			}

		private:
			template<typename T, typename TDistribuion>
			struct LastDist
			{
				T min;
				T max;
				TDistribuion* distribution;
			};

			class StrLess
			{
				public:
					bool operator()(const std::string& first, const std::string& second) const
					{
						return first.compare(second) < 0;
					}
			};

			TEngine _engine;
			std::map<std::string, std::uniform_int_distribution<int32_t>*, StrLess> _int32_cache;
			std::map<std::string, std::uniform_real_distribution<double>*, StrLess> _double_cache;
			std::uniform_int_distribution<int32_t> _int32_distribution;
			std::uniform_real_distribution<double> _double_distribution;
			struct LastDist<int32_t, std::uniform_int_distribution<int32_t>> _last_int32_dist;
			struct LastDist<double, std::uniform_real_distribution<double>> _last_double_dist;

			template<typename TDistribution, typename T>
			TDistribution* get_distribution(const T min, const T max, std::map<std::string, TDistribution*, StrLess>& cache, struct LastDist<T, TDistribution> &last_dist)
			{
				TDistribution* distribution;
				std::string key;
				std::ostringstream sstream;

				assert(min <= max);

				if(last_dist.min == min && last_dist.max == max)
				{
					return last_dist.distribution;
				}

				// create unique distribution key:
				sstream << min << "," << max;
				key = sstream.str();

				// try to find distribution in cache:
				auto iter = cache.find(key);

				if(iter == cache.end())
				{
					// create distribution if key does not exist:
					distribution = new TDistribution(min, max);
					cache[key] = distribution;
				}
				else
				{
					distribution = cache[key];
				}

				last_dist.min = min;
				last_dist.max = max;
				last_dist.distribution = distribution;

				return distribution;
			}

			template<typename T, typename TDistribution>
			void generate_sequence(TDistribution* distribution, T* numbers, const std::size_t length)
			{
				assert(numbers != nullptr);
				assert(length > 0);

				for(std::size_t i = 0; i < length; ++i)
				{
					numbers[i] = (*distribution)(_engine);
				}
			}

			template<typename TDistribution>
			void destroy_cache(std::map<std::string, TDistribution*, StrLess>& cache)
			{
				for(auto d : cache)
				{
					delete d.second;
				}
			}
	};

	/**
	   	@}
	   @}
	 */
}
#endif
