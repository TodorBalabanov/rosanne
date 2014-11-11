/*
 * slData.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef SLDATA_H_
#define SLDATA_H_

#include "Constants.h"
#include "slCell.h"

// Working data for computing the solution.
class slData {

public:

	slCell cells[slTOTAL_HANDS][slTOTAL_SUITS];

	int suit_total_length[slTOTAL_SUITS];
	int hand_total_length[slTOTAL_HANDS];
	int suit_allocated[slTOTAL_SUITS];
	int hand_allocated[slTOTAL_HANDS];

	int suit_sum_of_maxs[slTOTAL_SUITS];
	int hand_sum_of_maxs[slTOTAL_HANDS];

	// This is not really the sum of min values.
	// It is the sum of min values of vacant cells
	int suit_sum_of_vacant_mins[slTOTAL_SUITS];
	int hand_sum_of_vacant_mins[slTOTAL_HANDS];

	slData& operator=(const slData& value) {
		for(int i=0; i<slTOTAL_HANDS; i++) {
			for(int j=0; j<slTOTAL_SUITS; j++) {
				cells[i][j] = value.cells[i][j];
			}
		}

		for(int j=0; j<slTOTAL_SUITS; j++) {
			suit_total_length[j] = value.suit_total_length[j];
			suit_allocated[j] = value.suit_allocated[j];
			suit_sum_of_maxs[j] = value.suit_sum_of_maxs[j];
			suit_sum_of_vacant_mins[j] = value.suit_sum_of_vacant_mins[j];
		}

		for(int i=0; i<slTOTAL_HANDS; i++) {
			hand_total_length[i] = value.hand_total_length[i];
			hand_allocated[i] = value.hand_allocated[i];
			hand_sum_of_maxs[i] = value.hand_sum_of_maxs[i];
			hand_sum_of_vacant_mins[i] = value.hand_sum_of_vacant_mins[i];
		}

		return(*this);
	}

};

#endif /* SLDATA_H_ */
