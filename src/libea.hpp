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

#ifndef LIBEA_H
#define LIBEA_H

/**
   @addtogroup Core
   @{
 */

#include "AnsiRandomNumberGenerator.hpp"
#include "TR1UniformDistribution.hpp"

#include "PrimitiveGenome.hpp"

#include "algorithms.hpp"
#include "memory.hpp"
#include "SDBMHash.hpp"

#include "AFactory.hpp"

#include "FittestSelection.hpp"
#include "FitnessProportionalSelection.hpp"
#include "TournamentSelection.hpp"
#include "DoubleTournamentSelection.hpp"
#include "StochasticUniversalSampling.hpp"

#include "CutAndSpliceCrossover.hpp"
#include "CycleCrossover.hpp"
#include "EdgeRecombinationCrossover.hpp"
#include "OnePointCrossover.hpp"
#include "OrderedCrossover.hpp"
#include "PMXCrossover.hpp"
#include "TwoPointCrossover.hpp"
#include "UniformCrossover.hpp"

#include "SingleSwapMutation.hpp"
#include "DoubleSwapMutation.hpp"
#include "BitStringMutation.hpp"
#include "SingleBitStringMutation.hpp"
#include "InverseBitStringMutation.hpp"

#include "Pipeline.hpp"
#include "diversity.hpp"

/**
   @}
 */
#endif
