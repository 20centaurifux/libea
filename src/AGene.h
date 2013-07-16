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
/**
   @file AGene.h
   @brief Base class used for gene implementations.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */

#ifndef AGENE_H
#define AGENE_H

#include "Observable.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class AGeneListener
	   @brief Abstract base class for classes notified when an AGene has been modified.
	  */
	template<typename TGene>
	class AGeneListener
	{
		public:
			~AGeneListener() {};

			/**
			   @param gene gene that has been modified

			   Called then a gene has been modified.
			 */
			virtual void gene_modified(TGene* gene) = 0;
	};

	/**
	   @class AGene
	   @brief Abstract base class for equatable, cloneable, hashable and observable genes.
	 */
	template<typename TDerived>
	class AGene : public Observable<AGeneListener<TDerived>*>
	{
		public:
			/**
			   @struct LessThan
			   @brief A functor for comparing genes.
			 */
			typedef struct
			{
				/**
				    @param a a gene
				    @param b a gene
				    @return true if gene a is smaller than gene b
				  
				    Compares two genes.
				 */
				bool operator()(TDerived* a, TDerived* b)
				{
					return a->less_than(b);
				}

			} LessThan;

			virtual ~AGene() {}

			/**
			    @return hash of the gene
			  
			    Returns the hash of the gene.
			 */
			virtual size_t hash() = 0;

			/**
			    @param gene a gene to compare with instance
			    @return true if the instance is smaller than the given gene
			  
			    Compares two genes.
			 */
			virtual bool less_than(TDerived* gene) = 0;

			/**
			    @return a new gene instance
			  
			    Clones the instance.
			 */
			virtual TDerived* clone() const = 0;

			/**
			    @return a string representing the gene
			  
			    Returns a gene representing the gene.
			 */
			virtual std::string to_string() const = 0;

		protected:
			/**
			   Notify listeners that gene has been modified.
			 */
			void modified()
			{
				TDerived* gene = dynamic_cast<TDerived*>(this);

				this->for_each([&gene] (AGeneListener<TDerived>* l) { l->gene_modified(gene); });
			}
	};

	/**
	   @}
	 */
}
#endif
