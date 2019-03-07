#include <iostream>
#include <functional>
#include <algorithm>

#include "libea.hpp"

typedef int City;

struct Point
{
	int x;
	int y;
};

static const int N_CITIES = 59;

static Point Cities[N_CITIES] =
{
	{ 54, -65 }, { 0, 71 }, { -31, 53 }, { 8, 111 },
	{ 1, -9 }, { -36, 52 }, { -22, -76 }, { 0, 20 },
	{ 34, 129 }, { 28, 84 }, { 12, -38 }, { -21, -26 },
	{ -6, -41 }, { 21, 45 }, { 38, -90 }, { -24, 10 },
	{ -38, 35 }, { 86, -57 }, { 58, -1 }, { -9, -3 },
	{ 70, -74 }, { -20, 70 }, { -43, 44 }, { 59, -26 },
	{ -5, 114 }, { 83, -41 }, { 27, 153 }, { 12, -49 },
	{ 30, -65 }, { 31, -12 }, { -57, 28 }, { 44, -28 },
	{ 7, -7 }, { 54, -8 }, { 65, -8 }, { -35, 25 },
	{ 46, 79 }, { 5, 118 }, { 56,  4 }, { -21, 54 },
	{ -40, 45 }, { -43, 51 }, { 57, -21 }, { 0,  0 },
	{ 25, 15 }, { 56, -25 }, { -34, 56 }, { -24, 36 },
	{ -25, 49 }, { 64, -26 }, { 63, -48 }, { 37, 155 },
	{ -5, -24 }, { 2, 28 }, { -18, -58 }, { -10, 82 },
	{ 12, -58 }, { -40, -28 }, { -16, 28 }
};

using Route = std::vector<City>;
using Routes = std::vector<Route>;

static double
distance(const City a, const City b)
{
	return sqrt(pow(Cities[b].x - Cities[a].x, 2) + pow(Cities[b].y - Cities[a].y, 2));
}

static std::function<double(Route::iterator, Route::iterator)>
fitness = [](Route::iterator first, Route::iterator last)
{
	double f = 0.0;

	for (auto it = first; std::distance(it, last) > 2; it += 2)
	{
		f += distance(*it, *(it + 1));
	}

	return f;
};

template<typename InputIterator, typename Fitness>
static void debug(InputIterator first, InputIterator last, Fitness fitness)
{
	std::cout << "[ ";

	std::for_each(first, last, [](auto &g)
	{
		std::cout << g << " ";
	});

	std::cout << "] => " << fitness(first, last) << std::endl;
}

auto main() -> int
{
	// generate random routes:
	Routes routes;

	std::generate_n(std::back_inserter(routes), 30000, []()
	{
		Route route;

		ea::random::fill_distinct_n_int(std::back_inserter(route), N_CITIES, 0, N_CITIES - 1);

		return route;
	});

	// run operators:
	std::cout << "mean fitness: " << ea::fitness::mean(begin(routes), end(routes), fitness) << std::endl;
	std::cout << "median fitness: " << ea::fitness::median(begin(routes), end(routes), fitness) << std::endl;

	auto stream = ea::stream::make_mutable(begin(routes), end(routes));

	for(int i = 1; i <= 50; ++i)
	{
		stream = stream.select(ea::selection::DoubleTournament<std::less<double>>(), 1000, fitness)
		               .crossover(ea::crossover::PMX<Route>())
		               .mutate(ea::mutation::SingleSwap())
		               .mutate(ea::mutation::DoubleSwap());

		if(i % 5)
		{
			std::cout << i << " ..." << std::endl;
		}
		else
		{
			std::cout << i << "\t mean fitness: " << ea::fitness::mean(begin(stream), end(stream), fitness) << std::endl;
			std::cout << "\t median fitness: " << ea::fitness::median(begin(stream), end(stream), fitness) << std::endl;
		}
	}

	// show best route(s):
	routes.clear();

	stream.select(ea::selection::Fittest<std::less<double>>(), 3, fitness)
	      .take(std::back_inserter(routes));

	std::for_each(begin(routes), end(routes), [&](Route &route)
	{
		debug(begin(route), end(route), fitness);
	});

	// show best route & distances:
	auto best = *begin(routes);

	for(size_t i = 0; i < N_CITIES - 1; ++i)
	{
		std::cout << best[i] << " " << best[i + 1] << " " << distance(best[i], best[i + 1]) << std::endl;
	}
}

