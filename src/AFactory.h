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
 * \file AFactory.h
 * \brief Factories are used to create random instances of objects.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef AFACTORY_H
#define AFACTORY_H

#include <memory>
#include<vector>

#include "ARandomNumberGenerator.h"

namespace ea
{

	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class AFactory
	   @tparam T Datatype of objects the factory creates.
	   @brief Abstract base class for factories. Factories create random instances of objects.
	 */
	template<class T>
	class AFactory
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			AFactory(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : generator(rnd_generator) {}

			virtual ~AFactory() {}

			/**
			   @param count number of instances that should be created
			   @return a vector holding generated objects

			   Create new objects.
			 */
			virtual std::vector<T> random(const uint32_t count) = 0;

		protected:
			/*! A random number generator. */
			std::shared_ptr<ARandomNumberGenerator> generator;
	};

	/**
	   @}
	 */
}
#endif
