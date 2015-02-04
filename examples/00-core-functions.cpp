#include <iostream>
#include <libea.hpp>

// fitness function:
class Fitness
{
	public:
		float operator()(const ea::Sequence<int32_t>* const &seq) const
		{
			float avg = 0;

			for(auto i = 0; i < ea::sequence_len(seq); ++i)
			{
				avg += ea::sequence_get(seq, i);
			}

			return avg / ea::sequence_len(seq);
		}
};

// base class for modifying/accessing sequences:
static ea::PGenomeBase<int32_t, Fitness> base;

// print a sequence:
static void print_genome(ea::Sequence<int32_t>* seq)
{
	// print length, hash & fitness:
	std::cout << "len: " << base.len(seq) << ", hash: " << base.hash(seq) << ", fitness: " << base.fitness(seq) << std::endl;

	// print sequence:
	std::cout << "sequence: ";

	for(uint32_t i = 0; i < 10; ++i)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	// create new sequence:
	ea::Sequence<int32_t>* a = base.create(10);

	// set genes:
	for(uint32_t i = 0; i < 10; ++i)
	{
		base.set(a, i, i + 100);
	}

	std::cout << "genome a:" << std::endl;
	print_genome(a);

	// show index of genes:
	for (uint32_t g = 100; g <= 110; ++g)
	{
		std::cout << "index of " << g << ": " << base.index_of(a, g) << std::endl;
	}

	// copy sequence:
	ea::Sequence<int32_t>* b = base.copy(a);
	std::cout << "genome b:" << std::endl;
	print_genome(b);

	// compare sequences:
	std::cout << "compare: " << base.cmp(a, b) << std::endl;

	// change single gene:
	base.set(b, 5, 42);
	std::cout << "genome b:" << std::endl;
	print_genome(b);

	// compare sequences:
	std::cout << "compare: " << base.cmp(a, b) << std::endl;

	// free memory:
	ea::dispose(base, { a, b });

	return 0;
}
