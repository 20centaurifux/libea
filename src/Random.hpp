#ifndef EA_RANDOM_HPP
#define EA_RANDOM_HPP

#include <random>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <limits>

namespace ea::random
{
	using RandomEngine = std::mt19937;

	RandomEngine default_engine();

	template<typename InputIterator, typename T>
	void fill_n_int(InputIterator first, const size_t count, const T min, const T max)
	{
		RandomEngine eng = default_engine();
		std::uniform_int_distribution<T> dist(min, max);

		std::generate_n(first, count, [&dist, &eng]()
		{
			return dist(eng);
		});
	};

	template<typename InputIterator, typename T>
	void fill_n_real(InputIterator first, const size_t count, const T min, const T max)
	{
		RandomEngine eng = default_engine();
		std::uniform_real_distribution<T> dist(min, max);

		std::generate_n(first, count, [&dist, &eng]()
		{
			return dist(eng);
		});
	};

	template<typename InputIterator, typename T>
	void fill_distinct_n_int(InputIterator first, const size_t count, const T min, const T max)
	{
		if(count > 0)
		{
			if(min >= max)
			{
				throw std::invalid_argument("Invalid interval endpoints.");
			}

			// difference may overflow if min is less than zero:
			if(std::numeric_limits<T>::is_signed
			   && min < 0
			   && max > std::numeric_limits<T>::max() + min)
			{
				throw std::overflow_error("Interval endpoints out of range.");
			}

			T distance = max - min;

			if(static_cast<typename std::make_unsigned<T>::type>(distance) < count - 1) // interval is closed
			{
				throw std::invalid_argument("Number range too small.");
			}

			RandomEngine eng = default_engine();
			std::uniform_int_distribution<T> dist(min, max);
			std::vector<T> numbers;

			while(numbers.size() != count)
			{
				int n = dist(eng);

				if(std::find(begin(numbers), end(numbers), n) == end(numbers))
				{
					numbers.push_back(n);
				}
			}

			std::copy(begin(numbers), end(numbers), first);
		}
	};
}

#endif

