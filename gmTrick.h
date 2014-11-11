/*
 * gmTrick.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef GMTRICK_H_
#define GMTRICK_H_

#include "Constants.h"

class gmTrick {
public:

	bool trumped;
	int cards[gmTOTAL_PLAYERS];
	int lead_suit;
	int lead_loc;
	int count;
	int points;
	int winner;

	gmTrick& operator=(const gmTrick& value) {
		trumped = value.trumped;
		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			cards[i] = value.cards[i];
		}
		lead_suit = value.lead_suit;
		lead_loc = value.lead_loc;
		count = value.count;
		points = value.points;
		winner = value.winner;

		return(*this);
	}
};

#endif /* GMTRICK_H_ */
