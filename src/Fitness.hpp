#ifndef EA_FITNESS_HPP
#define EA_FITNESS_HPP

#include <map>
#include <vector>
#include <limits>
#include <iterator>
#include <cfenv>
#include <stdexcept>

namespace ea::fitness
{
	template<typename PopulationInputIterator, typename GenotypeInputIterator>
	std::function<double(PopulationInputIterator, const size_t)>
	fitness_by_index(std::function<double(GenotypeInputIterator, GenotypeInputIterator)> fn)
	{
		return [fn](PopulationInputIterator first, size_t index)
		{
			auto genotype = *(first + index);

			return fn(std::begin(genotype), std::end(genotype));
		};
	}

	template<typename PopulationInputIterator, typename GenotypeInputIterator>
	class MemoizeFitnessByIndex
	{
		public:
			MemoizeFitnessByIndex(std::function<double(GenotypeInputIterator, GenotypeInputIterator)> fn)
				: fn(fitness_by_index<PopulationInputIterator>(fn))
			{}

			double operator()(PopulationInputIterator population, const size_t index)
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
			 const std::function<double(PopulationInputIterator, const size_t)> fn;
			 std::map<size_t, double> cache;
	};

	template<typename PopulationInputIterator, typename GenotypeInputIterator>
	std::function<double(PopulationInputIterator, const size_t)>
	memoize_fitness_by_index(std::function<double(GenotypeInputIterator, GenotypeInputIterator)> fn)
	{
		return MemoizeFitnessByIndex<PopulationInputIterator, GenotypeInputIterator>(fn);
	}

	template<typename PopulationInputIterator, typename Fitness>
	double mean(PopulationInputIterator first, PopulationInputIterator last, Fitness fn)
	{
		double sum = 0.0;
		size_t size = 0;

		std::feclearexcept(FE_OVERFLOW);

		std::for_each(first, last, [&fn, &sum, &size](auto genotype)
		{
			sum += fn(begin(genotype), end(genotype));

			if(std::fetestexcept(FE_OVERFLOW))
			{
				throw std::overflow_error("Arithmetic overflow.");
			}

			++size;
		});

		return size > 0 ? sum / size
		                : std::numeric_limits<double>::quiet_NaN();
	}

	template<typename PopulationInputIterator, typename Fitness>
	double median(PopulationInputIterator first, PopulationInputIterator last, Fitness fn)
	{
		std::vector<double> values;

		std::for_each(first, last, [&values, &fn](auto g)
		{
			values.push_back(fn(begin(g), end(g)));
		});

		std::sort(begin(values), end(values));

		return values.size() > 0 ? values[(end(values) - begin(values)) / 2]
		                         : std::numeric_limits<double>::quiet_NaN();
	}

	template<typename PopulationInputIterator, typename Fitness, typename Compare = std::greater<double>>
	PopulationInputIterator fittest(PopulationInputIterator first, PopulationInputIterator last, Fitness fn)
	{
		using difference_type = typename std::iterator_traits<PopulationInputIterator>::difference_type;

		static Compare cmp;
		difference_type length = std::distance(first, last);
		difference_type index = 0;
		double fitness = 0.0;

		if(length > 0)
		{
			fitness = fn(begin(*first), end(*first));

			for(difference_type i = 1; i < length; ++i)
			{
				auto genotype = *(first + i);
				double new_fitness = fn(begin(genotype), end(genotype));

				if(cmp(fitness, new_fitness) < 1)
				{
					index = i;
					fitness = new_fitness;
				}
			};
		}

		return first + index;
	}
}

#endif

