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
 * \file AGene.h
 * \brief Base class used for gene implementations.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef AGENE_H
#define AGENE_H

#include "IEquatable.h"
#include "ICloneable.h"
#include "IHashfunction.h"
#include "Observable.h"

namespace ea
{
	class AGene;

	class AGeneListener
	{
		public:
			~AGeneListener() {};
			virtual void modified(const AGene* gene) = 0;
	};

	class AGene : public IEquatable<AGene>, public ICloneable<AGene>, public IHashfunction, public Observable<AGeneListener*>
	{
		public:
			struct equal_to
			{
				bool operator()(AGene* a, AGene* b)
				{
					return a->equals(b);
				}
			};

			struct hash_func
			{
				size_t operator()(AGene* gene)
				{
					return gene->hash();
				}
			};

			virtual ~AGene() {}

		protected:
			void notifiy()
			{
				for_each([gene = this] (AGeneListener* l) { l->modified(gene); });
			}
	};
}
#endif
