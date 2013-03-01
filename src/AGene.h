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
/**
   @file AGene.h
   @brief Base class used for gene implementations.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef AGENE_H
#define AGENE_H

#include <functional>

#include "IEquatable.h"
#include "ICloneable.h"
#include "IHashfunction.h"
#include "Observable.h"
#include "IToString.h"
#include "ACrossover.h"
#include "AIndexSelection.h"
#include "AMutation.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	class AGene;

	/**
	   @class AGeneListener
	   @brief Abstract base class for classes notified when an AGene has been modified.
	  */
	class AGeneListener
	{
		public:
			~AGeneListener() {};

			/**
			   @param gene gene that has been modified

			   Called then a gene has been modified.
			 */
			virtual void modified(const AGene* gene) = 0;
	};

	/**
	   @class AGene
	   @brief Abstract base class for equatable, cloneable, hashable and observable genes.
	 */
	class AGene : public IEquatable<AGene>, public ICloneable<AGene>, public IHashfunction, public Observable<AGeneListener*>, public IToString
	{
		public:
			/**
			   @struct equal_to
			   @brief Functor to compare two instances inherited from ABase.
			 */
			struct equal_to
			{
				/**
				   @param a a gene
				   @param b a gene
				   @return true of gene a and b are equal

				   Compares two genes.
				 */
				bool operator()(AGene* a, AGene* b) const
				{
					return a->equals(b);
				}
			};

			/**
			   @struct hash_func
			   @brief Functor to calculate hash of a gene inherited from ABase.
			 */
			struct hash_func
			{
				/**
				   @param gene a gene
				   @return hash value

				   Returns the hash value of a gene.
				 */
				size_t operator()(AGene* gene) const
				{
					return gene->hash();
				}
			};

			virtual ~AGene() {}

		protected:
			/**
			   Notify listeners that gene has been modified.
			 */
			void notifiy()
			{
				AGene* gene = this;

				for_each([&gene] (AGeneListener* l) { l->modified(gene); });
			}
	};

	/*! Crossover for genomes holding instances of AGene based classes. */
	typedef ACrossover<AGene*> AGeneCrossover;

	/*! Index selection for genomes holding instances of AGene based classes. */
	template<class Compare = std::greater<float> >
	struct AGeneIndexSelection
	{
		typedef AIndexSelection<AGene*, Compare> Type;
	};

	typedef AGeneIndexSelection<>::Type AGeneSelectMaximumIndex;

	typedef AGeneIndexSelection<std::less<float> >::Type AGeneSelectMinimumIndex;

	/*! Mutation for genomes holding instances of AGene based classes. */
	typedef AMutation<AGene*> AGeneMutation;

	/**
	   @}
	 */
}
#endif
