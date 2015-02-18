#include <stdint.h>
#include <iostream>
#include <vector>
#include <libea.hpp>

// store genomes in sequences of int32_t, this typedef makes the code more readable:
typedef ea::Sequence<int32_t> Sequence;

// fitness function:
class Fitness
{
	public:
		double operator()(const Sequence* const &seq) const
		{
			double avg = 0;

			for(ea::sequence_len_t i = 0; i < ea::sequence_len(seq); ++i)
			{
				avg += ea::sequence_get(seq, i);
			}

			return avg / ea::sequence_len(seq);
		}
};

static ea::Int32PGenomeBase<Fitness> base;

static void print_genome(Sequence* seq)
{
	for(ea::sequence_len_t i = 0; i < ea::sequence_len(seq); ++i)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	// create parent individuals:
	auto a = base.create(10);
	auto b = base.create(10);

	for(ea::sequence_len_t i = 0; i < 10; ++i)
	{
		base.set(a, i, i);
		base.set(b, i, 9 - i);
	}

	// create children:
	std::vector<Sequence*> children;
	auto adapter = ea::make_output_adapter(children);
	ea::CycleCrossover<ea::Int32PGenomeBase<Fitness>> crossover;

	std::size_t n = crossover.crossover(a, b, adapter);

	std::cout << "number of created children: " << n << std::endl;

	std::for_each(begin(children), end(children), [](Sequence* seq)
	{
		print_genome(seq);
	});

	// cleanup:
	ea::dispose(base, { a, b });
	ea::dispose(base, begin(children), end(children));

	return 0;
}
