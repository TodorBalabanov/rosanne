#ifndef GMINPUTTRUMPSELINFO_H_
#define GMINPUTTRUMPSELINFO_H_

class gmInputTrumpselInfo {

public:

	int card;
	int player;

	gmInputTrumpselInfo& operator=(const gmInputTrumpselInfo& value) {
		card = value.card;
		player = value.player;

		return(*this);
	}
};

#endif
