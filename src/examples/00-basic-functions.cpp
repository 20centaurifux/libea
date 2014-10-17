/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: sebastian.fedrau@gmail.com
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
 ***************************************************************************/
#include <stdint.h>
#include <iostream>
#include <PrimitiveGenome.hpp>

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
static ea::PrimitiveGenomeBase<int32_t, Fitness> base;

static void print_genome(Sequence* seq)
{
	// print length, hash & fitness:
	std::cout << "len: " << base.len(seq) << ", hash: " << base.hash(seq) << ", fitness: " << base.fitness(seq) << std::endl;

	// print sequence:
	std::cout << "sequence: ";

	for(uint32_t i = 0; i < 10; i++)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	// create new sequence:
	Sequence* a = base.create(10);

	// set genes:
	for(uint32_t i = 0; i < 10; i++)
	{
		base.set(a, i, i + 100);
	}

	std::cout << "genome a:" << std::endl;
	print_genome(a);

	// show index of genes:
	for (uint32_t g = 100; g <= 110; g++)
	{
		std::cout << "index of " << g << ": " << base.index_of(a, g) << std::endl;
	}

	// copy genome:
	Sequence* b = base.copy(a);
	std::cout << "genome b:" << std::endl;
	print_genome(b);

	// compare genomes:
	std::cout << "compare: " << base.cmp(a, b) << std::endl;

	// change single gene:
	base.set(b, 5, 42);
	std::cout << "genome b:" << std::endl;
	print_genome(b);

	// compare genomes:
	std::cout << "compare: " << base.cmp(a, b) << std::endl;

	// free memory:
	base.dispose(a);
	base.dispose(b);

	return 0;
}
