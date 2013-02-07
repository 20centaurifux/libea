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
   @file ea.h
   @brief General declarations.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef EA_H
#define EA_H

#include "AnsiRandomNumberGenerator.h"
#include "RandomDeviceNumberGenerator.h"
#include "URandomDeviceNumberGenerator.h"
#include "MersenneTwisterUniformIntDistribution.h"

#include "PrimitiveGenome.h"
#include "Genome.h"
#include "BinaryGenome.h"

#include "FitnessProportionalSelection.h"
#include "StochasticUniversalSampling.h"
#include "TournamentSelection.h"
#include "DoubleTournamentSelection.h"
#include "FittestSelection.h"

#include "CutAndSpliceCrossover.h"
#include "CycleCrossover.h"
#include "EdgeRecombinationCrossover.h"
#include "OnePointCrossover.h"
#include "OrderedCrossover.h"
#include "PMXCrossover.h"
#include "TwoPointCrossover.h"
#include "UniformCrossover.h"

#include "SingleSwapMutation.h"
#include "DoubleSwapMutation.h"

#include "BitStringMutation.h"
#include "SingleBitStringMutation.h"
#include "InverseBitStringMutation.h"

#include "AFactory.h"

/**
   @namespace ea
   @brief libea's namespace.
 */
namespace ea {}

#endif
