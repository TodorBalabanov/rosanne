/*
 * slProblem.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef SLPROBLEM_H_
#define SLPROBLEM_H_

#include "Constants.h"

// The data which represents the problem which is provided as one of the inputs to the
// suit length solver.
class slProblem {
public:

	slMatrix suit_length;
	int suit_total_length[slTOTAL_SUITS];
	int hand_total_length[slTOTAL_HANDS];

	slProblem& operator=(const slProblem& value) {
		for(int i=0; i<slTOTAL_HANDS; i++) {
			for(int j=0; j<slTOTAL_SUITS; j++) {
				suit_length[i][j] = value.suit_length[i][j];
			}
		}

		for(int j=0; j<slTOTAL_SUITS; j++) {
			suit_total_length[j] = value.suit_total_length[j];
		}

		for(int i=0; i<slTOTAL_HANDS; i++) {
			hand_total_length[i] = value.hand_total_length[i];
		}

		return(*this);
	}
};

#endif /* SLPROBLEM_H_ */
