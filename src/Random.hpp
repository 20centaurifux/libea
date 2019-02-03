#ifndef EA_RANDOM_HPP
#define EA_RANDOM_HPP

#include <random>
#include <algorithm>
#include <vector>

namespace ea::random
{
	using RandomEngine = std::mt19937;

	const RandomEngine &default_engine();

	template<class InputIterator>
	void fill_n(InputIterator first, const size_t count, const int min, const int max)
	{
		RandomEngine eng = default_engine();
		std::uniform_int_distribution<> dist(min, max);

		std::generate_n(first, count, [&dist, &eng]()
		{
			return dist(eng);
		});
	};

	template<class InputIterator>
	void fill_distinct_n(InputIterator first, const size_t count, const int min, const int max)
	{
		RandomEngine eng = default_engine();
		std::uniform_int_distribution<> dist(min, max);
		std::vector<int> numbers;

		while(numbers.size() != count)
		{
			int n = dist(eng);

			if(!std::any_of(begin(numbers), end(numbers), [&n](const int i) { return i == n; }))
			{
				numbers.push_back(n);
			}
		}

		std::copy(begin(numbers), end(numbers), first);
	};
}

#endif

