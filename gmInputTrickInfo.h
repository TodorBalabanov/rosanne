#ifndef GMINPUTTRICKINFO_H_
#define GMINPUTTRICKINFO_H_

class gmInputTrickInfo {

public:

	int player;
	bool can_ask_trump;
	bool ask_trump;
	unsigned long mask;
	int card;
	unsigned long rules;

	gmInputTrickInfo& operator=(const gmInputTrickInfo& value) {
		player = value.player;
		can_ask_trump = value.can_ask_trump;
		ask_trump = value.ask_trump;
		mask = value.mask;
		card = value.card;
		rules = value.rules;

		return(*this);
	}
};

#endif
