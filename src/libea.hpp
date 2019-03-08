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
   @file libea.hpp
   @brief One file to rule them all.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_LIBEA_HPP
#define EA_LIBEA_HPP

namespace ea
{
	/*! Major version. */
	const int LIBEA_MAJOR = 0;
	/*! Minor version. */
	const int LIBEA_MINOR = 3;
	/*! Patch level. */
	const int LIBEA_PATCHLEVEL = 0;
}

#include "Random.hpp"
#include "Diversity.hpp"
#include "Fitness.hpp"
#include "Utils.hpp"

#include "BitStringMutation.hpp"
#include "DoubleSwapMutation.hpp"
#include "InverseBitStringMutation.hpp"
#include "SingleBitStringMutation.hpp"
#include "SingleSwapMutation.hpp"

#include "EdgeRecombinationCrossover.hpp"
#include "CutAndSpliceCrossover.hpp"
#include "CycleCrossover.hpp"
#include "OnePointCrossover.hpp"
#include "OrderedCrossover.hpp"
#include "PMXCrossover.hpp"
#include "TwoPointCrossover.hpp"
#include "UniformCrossover.hpp"

#include "DoubleTournamentSelection.hpp"
#include "FitnessProportionalSelection.hpp"
#include "FittestSelection.hpp"
#include "StochasticUniversalSampling.hpp"
#include "TournamentSelection.hpp"

#include "Stream.hpp"

#endif

