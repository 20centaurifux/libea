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
		float operator()(const Sequence* const &seq) const
		{
			float avg = 0;

			for(auto i = 0; i < ea::sequence_len(seq); i++)
			{
				avg += ea::sequence_get(seq, i);
			}

			return avg / ea::sequence_len(seq);
		}
};

static ea::Int32PGenomeBase<Fitness> base;

static void print_genome(Sequence* seq)
{
	for(uint32_t i = 0; i < ea::sequence_len(seq); i++)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	ea::AnsiRandomNumberGenerator g;

	// create parent individuals:
	std::vector<Sequence*> population;

	for(uint32_t i = 0; i < 10; i++)
	{
		auto seq = base.create(10);

		int32_t genes[10];
		g.get_unique_int32_seq(0, 9, genes, 10);

		for(uint32_t j = 0; j < 10; j++)
		{
			base.set(seq, j, genes[j]);
		}

		population.push_back(seq);
	}

	// create input adapter:
	auto input = ea::make_input_adapter(population);

	// print parent individuals:
	std::for_each(begin(population), end(population), [](Sequence* seq)
	{
		print_genome(seq);
	});

	std::cout << std::endl;

	// select & print individuals:
	std::vector<uint32_t> children;
	auto inserter = std::back_inserter(children);
	ea::STLVectorAdapter<uint32_t> output(inserter);

	ea::TournamentSelection<ea::Int32PGenomeBase<Fitness>> sel;
	sel.select(input, 5, output);

	std::for_each(begin(children), end(children), [&population](uint32_t index)
	{
		std::cout << "selected child: " << index << " ==> ";
		print_genome(population[index]);
	});

	// cleanup:
	ea::dispose(base, begin(population), end(population));

	return 0;
}
