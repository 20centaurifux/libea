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
/*!
 * \file AFactory.h
 * \brief Factories are used to create random instances of objects.
 * \author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef AFACTORY_H
#define AFACTORY_H

#include <memory>
#include "OutputAdapter.h"
#include "ARandomNumberGenerator.h"

namespace ea
{

	/**
	   @addtogroup Core
	   @{
	 */

	/**
	   @class AFactory
	   @tparam T datatype of objects created by the factory
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
			   @param container container to write generated objects to
			   @tparam TContainer type of the specified container

			   Creates random objects.
			 */
			template<typename TContainer>
			void random(const uint32_t count, TContainer &container)
			{
				auto adapter = make_output_adapter<typename TContainer::value_type>(container);

				random_impl(count, adapter);
			}

		protected:
			/*! A random number generator. */
			std::shared_ptr<ARandomNumberGenerator> generator;

			/**
			   @param count number of objects to create
			   @param output destination to write created objects to
			 */
			virtual void random_impl(const uint32_t count, IOutputAdapter<T> &output) = 0;
	};

	/**
	   @}
	 */
}
#endif
