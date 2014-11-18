#ifndef GMINPUTBIDINFO_H_
#define GMINPUTBIDINFO_H_

class gmInputBidInfo {
public:

	int player;
	int min;
	bool passable;
	int bid;
	int round;

	gmInputBidInfo& operator=(const gmInputBidInfo& value) {
		player = value.player;
		min = value.min;
		passable = value.passable;
		bid = value.bid;
		round = value.round;

		return(*this);
	}
};

#endif
