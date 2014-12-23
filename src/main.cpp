#include <stdint.h>
#include <iostream>
#include <vector>
#include <libea.hpp>
#include <Pipeline.hpp>

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

// base class for modifying/accessing sequences:
typedef ea::PGenomeBase<int32_t, Fitness> Base;

static Base base;

// factory class:
class Factory : public ea::AFactory<Sequence*>
{
	public:
		Sequence* create_sequence()
		{
			int32_t genes[10];

			_g.get_int32_seq(1, 100, genes, 10);

			Sequence* seq = base.create(10);

			for(uint32_t i = 0; i < 10; i++)
			{
				base.set(seq, i, genes[i]);
			}

			return seq;
		}

	private:
		static ea::AnsiRandomNumberGenerator _g;
};

ea::AnsiRandomNumberGenerator Factory::_g;

// debug function:
/*
static void print_genome(Sequence* seq)
{
	std::cout << "hash: " << base.hash(seq) << ", fitness: " << base.fitness(seq) << ", sequence: ";

	for(uint32_t i = 0; i < 10; i++)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}
*/

int main(int argc, char *argv[])
{
	Factory f;
	std::vector<Sequence*> population;

	// create adapter from back_inserter:
	auto inserter = std::back_inserter(population);
	ea::STLVectorAdapter<Sequence*> adapter(inserter);

	// create 100 genomes:
	f.create_population(100, adapter);

	auto source = ea::make_input_adapter(population);

	std::vector<Sequence*> children;
	auto cinserter = std::back_inserter(children);
	ea::STLVectorAdapter<Sequence*> cadapter(cinserter);

	auto rnd = std::make_shared<ea::AnsiRandomNumberGenerator>();

	auto selection_a = ea::SelectionPipelineElement<Base, ea::SourceDivisor<Base>>(new ea::TournamentSelection<Base>());
	auto selection_b = ea::SelectionPipelineElement<Base, ea::FixedSelectionSize<Base, 50>>(new ea::DoubleTournamentSelection<Base>());
	auto crossover = ea::CrossoverPipelineElement<Base>(new ea::CutAndSpliceCrossover<Base>());
	auto mutation = ea::MutationPipelineElement<Base>(new ea::SingleSwapMutation<Base>(), rnd);
	auto terminator = ea::ForLoopTerminator<Base>(1);
	ea::ITerminator<Base>& terminator_ref = terminator;

	uint32_t sum = ea::pipeline_process<Base>(source, cadapter, { &selection_a, &crossover, &mutation, &selection_b }, terminator_ref);

	std::cout << sum << std::endl;

	ea::dispose(base, begin(population), end(population));
	ea::dispose(base, begin(children), end(children));

	return 0;
}
