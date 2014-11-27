#include <iostream>
#include <libea.hpp>

// store genomes in sequences of int32_t, this typedef makes the code more readable:
typedef ea::Sequence<bool> Sequence;

// fitness function:
class Fitness
{
	public:
		float operator()(const Sequence* const &seq) const
		{
			float count = 0;

			for(auto i = 0; i < ea::sequence_len(seq); i++)
			{
				if(ea::sequence_get(seq, i))
				{
					count++;
				}
			}

			return count;
		}
};

typedef ea::BinaryPGenomeBase<Fitness> Base;
static ea::BinaryPGenomeBase<Fitness> base;

static void print_genome(Sequence* seq)
{
	for(uint32_t i = 0; i < ea::sequence_len(seq); i++)
	{
		std::cout << (base.get(seq, i) ? 1 : 0);
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	// create parent individual:
	auto a = base.create(10);

	for(uint32_t i = 0; i < 10; i++)
	{
		base.set(a, i, i % 3);
	}

	print_genome(a);

	// inplace mutation:
	ea::BitStringMutation<Base> muta;

	muta.mutate(a);
	print_genome(a);

	// create child:
	ea::InverseBitStringMutation<Base> mutb;

	auto b = mutb.create_child(a);
	print_genome(b);

	// cleanup:
	ea::dispose(base, { a, b });

	return 0;
}
