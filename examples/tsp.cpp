#include <iostream>
#include <functional>
#include <algorithm>
#include <cstdint>
#include <map>

#include "libea.hpp"

typedef uint8_t City;

struct Point
{
	uint16_t x;
	uint16_t y;
};

static std::map<City, Point> Cities =
{
	{ 0, { 31, 9 } },
	{ 1, { 17, 82 } },
	{ 2, { 56, 47 } },
	{ 3, { 70,  3 } },
	{ 4, { 11,  7 } },
	{ 5, { 93, 83 } },
	{ 6, { 96, 22 } },
	{ 7, { 26, 97 } },
	{ 8, { 52, 58 } },
	{ 9, { 68, 6 } }
};

using Route = std::vector<City>;
using Routes = std::vector<Route>;

static std::function<double(Route::iterator, Route::iterator)>
fitness = [](Route::iterator first, Route::iterator last)
{
	double f = 0.0;

	for (auto it = first; std::distance(it, last) > 2; it += 2)
	{
		f += sqrt(pow(Cities[*(it + 1)].x - Cities[*it].x, 2) + pow(Cities[*(it + 1)].y - Cities[*it].y, 2));
	}

	return f;
};

template<typename InputIterator, typename Fitness>
static void debug(InputIterator first, InputIterator last, Fitness fitness)
{
	std::cout << "[ ";

	std::for_each(first, last, [](auto &g)
	{
		std::cout << (int)g << " ";
	});

	std::cout << "] => " << fitness(first, last) << std::endl;
}

auto main() -> int
{
	// generate population:
	Routes routes;

	std::generate_n(std::back_inserter(routes), 30000, []()
	{
		Route route;

		ea::random::fill_distinct_n_int(std::back_inserter(route), 10, 0, 9);

		return route;
	});

	// run operators:
	std::cout << "mean fitness: " << ea::fitness::mean(begin(routes), end(routes), fitness) << std::endl;
	std::cout << "median fitness: " << ea::fitness::median(begin(routes), end(routes), fitness) << std::endl;

	auto stream = ea::stream::make_mutable(begin(routes), end(routes));

	for(int i = 1; i <= 10; ++i)
	{
		stream = stream.select(ea::selection::DoubleTournament<std::less<double>>(), 250, fitness)
		               .crossover(ea::crossover::PMX<Route>())
		               .mutate(ea::mutation::SingleSwap())
		               .mutate(ea::mutation::DoubleSwap());

		std::cout << i << ".\t mean fitness: " << ea::fitness::mean(begin(stream), end(stream), fitness) << std::endl;
		std::cout << " \t median fitness: " << ea::fitness::median(begin(stream), end(stream), fitness) << std::endl;
	}

	// show best route(s):
	routes.clear();

	stream.select(ea::selection::Fittest<std::less<double>>(), 3, fitness)
	      .take(std::back_inserter(routes));

	std::for_each(begin(routes), end(routes), [&](Route &route)
	{
		debug(begin(route), end(route), fitness);
	});
}

