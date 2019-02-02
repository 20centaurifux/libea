#ifndef EA_FITNESS_HPP
#define EA_FITNESS_HPP

#include <map>

namespace ea::utils
{
	template<typename PopulationInputIterator, typename GenotypeInputIterator>
	std::function<double(const PopulationInputIterator, const size_t)>
	fitness_by_index(const std::function<double(const GenotypeInputIterator, const GenotypeInputIterator)> fn)
	{
		return [fn](const PopulationInputIterator first, const size_t index)
		{
			auto genotype = *(first + index);

			return fn(std::begin(genotype), std::end(genotype));
		};
	}

	template<typename PopulationInputIterator, typename GenotypeInputIterator>
	class MemoizeFitnessByIndex
	{
		public:
			MemoizeFitnessByIndex(const std::function<double(const GenotypeInputIterator, const GenotypeInputIterator)> fn)
				: fn(fitness_by_index<PopulationInputIterator>(fn))
			{}

			double operator()(const PopulationInputIterator population, const size_t index)
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
			 const std::function<double(const PopulationInputIterator, const size_t)> fn;
			 std::map<size_t, double> cache;
	};

	template<typename PopulationInputIterator, typename GenotypeInputIterator>
	std::function<double(const PopulationInputIterator, const size_t)> memoize_fitness_by_index(const std::function<double(const GenotypeInputIterator, const GenotypeInputIterator)> fn)
	{
		return MemoizeFitnessByIndex<PopulationInputIterator, GenotypeInputIterator>(fn);
	}
}

#endif
