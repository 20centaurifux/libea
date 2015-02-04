#include <libea.hpp>
#include <vector>
#include <map>
#include <iostream>

using namespace std;
using namespace ea;
using namespace ea::Pipeline;

// type declarations:
typedef uint8_t City;
typedef CSequence<City>* Route;

typedef struct
{
	uint16_t x;
	uint16_t y;
} Point;

// city map:
static map<City, Point> Cities = {
	{ 0, { 31, 9 } },
	{ 1, { 17, 82 } },
	{ 2, { 56, 47 } },
	{ 3, { 70,  3 } },
	{ 4, { 11,  7 } },
	{ 5, { 93, 83 } },
	{ 6, { 96, 22 } },
	{ 7, { 26, 97 } },
	{ 8, { 52, 58 } },
	{ 9, { 68,  6 } } };

// fitness:
class Fitness
{
	public:
		float operator()(const Route &seq) const
		{
			float f = 0;

			for(auto i = 0; i < sequence_len(seq) - 1; ++i)
			{
				auto a = sequence_get(seq, i);
				auto b = sequence_get(seq, i + 1);

				f += sqrt(pow(Cities[b].x - Cities[a].x, 2) + pow(Cities[b].y - Cities[a].y, 2));
			}

			return f;
		}
};

// base class type declaration:
typedef CPGenomeBase<City, Fitness> Base;

// factory:
class RouteFactory : public ea::AFactory<Route>
{
	public:
		Route create_sequence()
		{
			int32_t genes[10];

			_g.get_unique_int32_seq(0, 9, genes, 10);

			auto seq = _base.create(10);

			for(uint32_t i = 0; i < 10; ++i)
			{
				_base.set(seq, i, genes[i]);
			}

			return seq;
		}

	private:
		static AnsiRandomNumberGenerator _g;
		static Base _base;
};

AnsiRandomNumberGenerator RouteFactory::_g;
Base RouteFactory::_base;

// process pipeline:
int
main(int argc, char *argv[])
{
	Base base;
	RouteFactory f;
	vector<Route> population;
	auto rnd = make_shared<ea::AnsiRandomNumberGenerator>();

	// create 100 routes:
	auto a = make_output_adapter(population);

	f.create_population(100, a);

	// create and process pipeline:
	auto source = make_input_adapter(population);

	cout << "mean fitness (parent): " << mean<Base>(source) << endl;
	cout << "median fitness (parent): " << median<Base>(source) << endl;

	vector<Route> children;
	auto cadapter = make_output_adapter(children);

	auto selection_a =
		SelectionElement<Base, SourceDivisor<Base>>
			(new TournamentSelection<Base, std::less<double>>());

	auto selection_b =
		SelectionElement<Base, FixedSelectionSize<Base, 50>>(
			new DoubleTournamentSelection<Base, std::less<double>>());

	auto crossover =
		CrossoverElement<Base>
			(new EdgeRecombinationCrossover<Base>());

	auto mutation =
		MutationElement<Base>
			(new SingleSwapMutation<Base>(), rnd);

	auto terminator = ForLoopTerminator<Base>(100);
	ITerminator<Base>& terminator_ref = terminator;

	pipeline_process<Base>(source, cadapter, { &selection_a, &crossover, &selection_b, &mutation }, terminator_ref);

	source = make_input_adapter(children);

	cout << "mean fitness (parent): " << mean<Base>(source) << endl;
	cout << "median fitness (parent): " << median<Base>(source) << endl;

	// cleanup:
	dispose(base, begin(population), end(population));
	dispose(base, begin(children), end(children));

	return 0;
}
