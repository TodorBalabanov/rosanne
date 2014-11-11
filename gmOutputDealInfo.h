/*
 * gmOutputDealInfo.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef GMOUTPUTDEALINFO_H_
#define GMOUTPUTDEALINFO_H_

#include "Constants.h"

class gmOutputDealInfo {
public:

	int round;
	unsigned long hands[gmTOTAL_PLAYERS];

	gmOutputDealInfo& operator=(const short value) {
		if(value != (short)0) {
			return(*this);
		}

		round = 0;
		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			hands[i] = 0;
		}

		return(*this);
	}

	gmOutputDealInfo& operator=(const gmOutputDealInfo& value) {
		round = value.round;

		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			hands[i] = value.hands[i];
		}

		return(*this);
	}
};

#endif /* GMOUTPUTDEALINFO_H_ */
