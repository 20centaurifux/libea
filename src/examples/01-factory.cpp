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
#include <vector>
#include <PrimitiveGenome.hpp>
#include <AFactory.hpp>
#include <AnsiRandomNumberGenerator.hpp>

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
static void print_genome(Sequence* seq)
{
	std::cout << "hash: " << base.hash(seq) << ", fitness: " << base.fitness(seq) << ", sequence: ";

	for(uint32_t i = 0; i < 10; i++)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	Factory f;
	std::vector<Sequence*> population;

	// create adapter from back_inserter:
	auto inserter = std::back_inserter(population);
	ea::STLVectorAdapter<Sequence*> adapter(inserter);

	// create 100 genomes:
	f.create_population(100, adapter);

	// print genomes & free memory:
	std::for_each(begin(population), end(population), [](Sequence* seq)
	{
		print_genome(seq);
	});

	ea::dispose(base, begin(population), end(population));

	return 0;
}
