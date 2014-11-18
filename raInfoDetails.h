#ifndef RAINFODETAILS_H_
#define RAINFODETAILS_H_

#include "Constants.h"

class raInfoDetails {
public:
	int deal_no;
	int dealer;
	int bid;
	int bidder;
	int trump;
	int points[gmTOTAL_TEAMS];
	int pnlties[gmTOTAL_PLAYERS];

	raInfoDetails& operator=(const short value) {
		if(value != (short)0) {
			return(*this);
		}

		deal_no = 0;
		dealer = 0;
		bid = 0;
		bidder = 0;
		trump = 0;
		for(int i=0; i<gmTOTAL_TEAMS; i++) {
			points[i] = 0;
		}
		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			pnlties[i] = 0;
		}

		return(*this);
	}

	raInfoDetails& operator=(const raInfoDetails& value) {
		deal_no = value.deal_no;
		dealer = value.dealer;
		bid = value.bid;
		bidder = value.bidder;
		trump = value.trump;
		for(int i=0; i<gmTOTAL_TEAMS; i++) {
			points[i] = value.points[i];
		}
		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			pnlties[i] = value.pnlties[i];
		}

		return(*this);
	}
};

#endif
