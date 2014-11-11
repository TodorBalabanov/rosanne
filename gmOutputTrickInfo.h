/*
 * gmOutputTrickInfo.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef GMOUTPUTTRICKINFO_H_
#define GMOUTPUTTRICKINFO_H_

#include "Constants.h"
#include "gmTrick.h"

class gmOutputTrickInfo {
public:
	int points[gmTOTAL_TEAMS];
	gmTrick trick;
};

#endif /* GMOUTPUTTRICKINFO_H_ */
