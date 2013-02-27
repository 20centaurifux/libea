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
   @file VectorAdapter.h
   @brief Adapter class for STL vectors.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef VECTORADAPTER_H
#define VECTORADAPTER_H

#include "IInserter.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class VectorAdapter
	   @tparam Iterator type of the STL iterator
	   @tparam T type of the value stored in the related container
	   @brief Adapter class for STL iterators.
	 */
	template<class Vector, class T>
	class VectorAdapter : public IInserter<T>
	{
		public:
			/**
			   @param vector a vector
			 */
			VectorAdapter(Vector& vector) : _vector(vector) {}

			void insert(T genome)
			{
				_vector.push_back(genome);
			}

		private:
			Vector& _vector;
	};

	/**
	   @}
	 */
}
#endif
