#ifndef EA_UTILS_HPP
#define EA_UTILS_HPP

namespace ea::utils
{
	template<typename F>
	void repeat(const size_t count, F fn)
	{
		for(size_t i = 0; i < count; ++i)
		{
			fn();
		}
	}
}

#endif

