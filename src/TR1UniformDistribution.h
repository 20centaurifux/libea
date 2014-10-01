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
   @file TR1UniformDistribution.h
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
#include <limits>
#include <mutex>
#include "ARandomNumberGenerator.h"

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
	template<typename TEngine>
	class TR1UniformDistribution : public ARandomNumberGenerator
	{
		public:
			TR1UniformDistribution()
			{
				#ifdef THREAD_SAFE
				std::lock_guard<std::mutex> lock(_mutex);

				if(!_seeded)
				{
					_engine.seed(time(nullptr));
					_seeded = true;

					memset(&_last_int32_dist, 0, sizeof(_last_int32_dist));
					memset(&_last_double_dist, 0, sizeof(_last_double_dist));
				}
				#else
					_engine.seed(time(nullptr));

					memset(&_last_int32_dist, 0, sizeof(_last_int32_dist));
					memset(&_last_double_dist, 0, sizeof(_last_double_dist));
				#endif
			}

			virtual ~TR1UniformDistribution()
			{
				//destroy_cache(_int32_cache);
				//destroy_cache(_double_cache);
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
				#ifdef THREAD_SAFE
				std::lock_guard<std::mutex> lock(_mutex);
				#endif

				return (*get_distribution<std::uniform_int_distribution<int32_t>>(min, max, _int32_cache, _last_int32_dist))(_engine);
			}

			double get_double(const double min, const double max) override
			{
				#ifdef THREAD_SAFE
				std::lock_guard<std::mutex> lock(_mutex);
				#endif

				return (*get_distribution<std::uniform_real_distribution<double>>(min, max, _double_cache, _last_double_dist))(_engine);
			}

			void get_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length) override
			{
				#ifdef THREAD_SAFE
				std::lock_guard<std::mutex> lock(_mutex);
				#endif

				generate_sequence(get_distribution<std::uniform_int_distribution<int32_t>>(min, max, _int32_cache, _last_int32_dist), numbers, length);
			}

			void get_double_seq(const double min, const double max, double* numbers, const int32_t length) override
			{
				#ifdef THREAD_SAFE
				std::lock_guard<std::mutex> lock(_mutex);
				#endif

				generate_sequence(get_distribution<std::uniform_real_distribution<double>>(min, max, _double_cache, _last_double_dist), numbers, length);
			}

			inline int32_t get_max_int32() const override
			{
				return std::numeric_limits<int32_t>::max();
			}

			double get_max_double() const override
			{
				return std::numeric_limits<double>::max();
			}

		private:
			template<typename T, typename TDistribuion>
			struct LastDist
			{
				T min;
				T max;
				TDistribuion* distribution;
			};

			static TEngine _engine;

			#ifdef THREAD_SAFE
			static std::mutex _mutex;
			static bool _seeded;
			#endif

			static std::ostringstream _sstream;
			static std::map<std::string, std::uniform_int_distribution<int32_t>*> _int32_cache;
			static std::map<std::string, std::uniform_real_distribution<double>*> _double_cache;
			static std::uniform_int_distribution<int32_t> _int32_distribution;
			static std::uniform_real_distribution<double> _double_distribution;
			static struct LastDist<int32_t, std::uniform_int_distribution<int32_t>> _last_int32_dist;
			static struct LastDist<double, std::uniform_real_distribution<double>> _last_double_dist;

			template<typename TDistribution, typename T>
			static TDistribution* get_distribution(const T min, const T max, std::map<std::string, TDistribution*>& cache, struct LastDist<T, TDistribution> &last_dist)
			{
				TDistribution* distribution;
				std::string key;

				assert(min < max);

				if(last_dist.min == min && last_dist.max == max)
				{
					return last_dist.distribution;
				}

				// create unique distribution key:
				_sstream.clear();
				_sstream << min << "," << max;
				key = _sstream.str();

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
			static void generate_sequence(TDistribution* distribution, T* numbers, const int32_t length)
			{
				assert(numbers != nullptr);
				assert(length > 0);

				for(int32_t i = 0; i < length; i++)
				{
					numbers[i] = (*distribution)(_engine);
				}
			}

			template<typename TDistribution>
			static void destroy_cache(std::map<std::string, TDistribution*>& cache)
			{
				for(auto d : cache)
				{
					delete d.second;
				}
			}
	};

	template<typename TEngine>
	TEngine TR1UniformDistribution<TEngine>::_engine;

	#ifdef THREAD_SAFE
	template<typename TEngine>
	std::mutex TR1UniformDistribution<TEngine>::_mutex;

	template<typename TEngine>
	bool TR1UniformDistribution<TEngine>::_seeded = false;
	#endif

	template<typename TEngine>
	std::ostringstream TR1UniformDistribution<TEngine>::_sstream;

	template<typename TEngine>
	std::map<std::string, std::uniform_int_distribution<int32_t>*> TR1UniformDistribution<TEngine>::_int32_cache;

	template<typename TEngine>
	std::map<std::string, std::uniform_real_distribution<double>*> TR1UniformDistribution<TEngine>::_double_cache;

	template<typename TEngine>
	std::uniform_int_distribution<int32_t> TR1UniformDistribution<TEngine>::_int32_distribution;

	template<typename TEngine>
	std::uniform_real_distribution<double> TR1UniformDistribution<TEngine>::_double_distribution;

	template<typename TEngine>
	struct TR1UniformDistribution<TEngine>::LastDist<int32_t, std::uniform_int_distribution<int32_t>> TR1UniformDistribution<TEngine>::_last_int32_dist;

	template<typename TEngine>
	struct TR1UniformDistribution<TEngine>::LastDist<double, std::uniform_real_distribution<double>> TR1UniformDistribution<TEngine>::_last_double_dist;

	/**
	   	@}
	   @}
	 */
}
#endif
