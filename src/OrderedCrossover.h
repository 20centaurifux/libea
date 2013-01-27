/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/*!
 * \file OrderedCrossover.h
 * \brief Implementation of ordered crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef ORDEREDCROSSOVER_H
#define ORDEREDCROSSOVER_H

#include <functional>
#include "ACrossover.h"

namespace ea
{
	template<class T, class Equals = std::equal_to<T> >
	class OrderedCrossover : public ACrossover<T>
	{
		public:
			OrderedCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover<T>(rnd_generator) {}
			~OrderedCrossover() {};

			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, std::vector<AGenome<T>*>& children)
			{
				uint32_t separator;
				uint32_t i;
				uint32_t m = 0;
				AGenome<T> *individual;

				separator = (uint32_t)ACrossover<T>::generator->get_number(1, a->size() - 2);

				individual = a->instance();

				for(i = 0; i < separator; i++)
				{
					individual->copy_to(i, a->at(i));
				}

				for(i = separator; i < individual->size(); ++i)
				{
					while(gene_exists(b->at(m), individual, i))
					{
						++m;
					}

					assert(m < b->size());

					individual->copy_to(i, b->at(m));
				}

				children.push_back(individual);

				return 1;
			}

		private:
			Equals equals;

			inline bool gene_exists(T gene, AGenome<T>* individual, const uint32_t len)
			{
				for(uint32_t i = 0; i < len; ++i)
				{
					if(equals(gene, individual->at(i)))
					{
						return true;
					}
				}

				return false;
			}
	};
}
#endif
