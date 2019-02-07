#include "Random.hpp"

namespace ea::random
{
	RandomEngine default_engine()
	{
		static std::random_device rd;
		std::mt19937 mt(rd());

		return mt;
	}
}

