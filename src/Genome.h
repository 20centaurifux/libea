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
 * \file Genome.h
 * \brief A genome holds multiple genes.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include "AGenome.h"
#include "Gene.h"
#include "IHashfunction.h"
#include "IListener.h"
#include "IObservable.h"

namespace ea
{
	class Genome;

	class GenomeEventArg : public IListener
	{
		public:
			uint32_t offset;
			Gene* gene;
	};

	class GenomeListener : public IListener
	{
		public:
			virtual ~GenomeListener() {};
			virtual void created(const Genome* sender, const GenomeEventArg* arg) {}
			virtual void modified(const Genome* sender, const GenomeEventArg* arg) {}
			virtual void deleted(const Genome* sender, const GenomeEventArg* arg) {}
			virtual void gene_set(const Genome* sender, const GeneEventArg* arg) {}
			virtual void gene_cleared(const Genome* sender) {}
	};

	class Genome : public AGenome<Gene*>, public IObservable<GenomeListener>, public GeneListener
	{
		public:
			Genome(const uint32_t size);
			virtual ~Genome();
			inline uint32_t size() const { return _genes->size(); }

			void set(const uint32_t index, Gene* gene);
			void copy_to(const uint32_t index, Gene* gene);
			Gene* at(const uint32_t index) const;
			bool find(Gene* gene, uint32_t& index) const;
			bool contains(Gene* gene) const;
			void swap(const uint32_t pos1, const uint32_t pos2) const;
			size_t hash() const;

			void attach_listener(GenomeListener* l)
			{
				listener.push_back(l);
			}

			void detach_listener(GenomeListener* l)
			{
				detach_helper(listener, l);
			}

			void modified(const Gene* sender, const GeneEventArg* arg);
			void cleared(const Gene* sender);

		private:
			typedef enum
			{
				GENOME_EVENT_SET,
				GENOME_EVENT_ADDED,
				GENOME_EVENT_REMOVED
			} GENOME_EVENT;

			std::vector<Gene*>* _genes;
			mutable size_t _hash;
			mutable bool _hash_set;
			std::vector<GenomeListener*> listener;

			void invoke_listener(GENOME_EVENT event, const uint32_t offset);
	};
}
#endif
