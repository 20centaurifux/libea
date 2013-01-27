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
 * \file UniformCrossover.cpp
 * \brief Implementation of uniform crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef UNIFORMCROSSOVER_H
#define UNIFORMCROSSOVER_H

#include "ACrossover.h"

namespace ea
{
	template<class T, int N = 2>
	class UniformCrossover : public ACrossover<T>
	{
		public:
			UniformCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover<T>(rnd_generator) {}
			virtual ~UniformCrossover() {};

			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, std::vector<AGenome<T>*>& children)
			{
				AGenome<T>* child1;
				AGenome<T>* child2;
				int32_t rnd;

				child1 = a->instance();
				child2 = a->instance();

				for(uint32_t i = 0; i < a->size(); ++i)
				{
					do
					{
						rnd = ACrossover<T>::generator->random();
					} while(!rnd);

					if(rnd % N)
					{
						child1->copy_to(i, a->at(i));
						child2->copy_to(i, b->at(i));
					}
					else
					{
						child1->copy_to(i, b->at(i));
						child2->copy_to(i, a->at(i));
					}
				}

				children.push_back(child1);
				children.push_back(child2);

				return 2;
			}
	};
}
#endif
