#include "Random.hpp"

namespace ea::random
{
	const RandomEngine &default_engine()
	{
		static std::random_device rd;
		static std::mt19937 mt(rd());

		return mt;
	}
}

