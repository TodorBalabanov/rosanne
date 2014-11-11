/*
 * gmRules.h
 *
 *  Created on: Nov 11, 2014
 *      Author: student
 */

#ifndef GMRULES_H_
#define GMRULES_H_

class gmRules {
public:
	int rot_addn;
	int min_bid_1;
	int min_bid_2;
	int min_bid_3;
	bool waive_rule_4;
	bool sluff_jacks;

	gmRules& operator=(const short value) {
		if(value != (short)0) {
			return(*this);
		}

		rot_addn = 0;
		min_bid_1 = 0;
		min_bid_2 = 0;
		min_bid_3 = 0;
		waive_rule_4 = false;
		sluff_jacks = false;

		return(*this);
	}

	gmRules& operator=(const gmRules& value) {
		rot_addn = value.rot_addn;
		min_bid_1 = value.min_bid_1;
		min_bid_2 = value.min_bid_2;
		min_bid_3 = value.min_bid_3;
		waive_rule_4 = value.waive_rule_4;
		sluff_jacks = value.sluff_jacks;

		return(*this);
	}
};

#endif /* GMRULES_H_ */
