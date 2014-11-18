#ifndef GMOUTPUTDEALENDINFO_H_
#define GMOUTPUTDEALENDINFO_H_

class gmOutputDealEndInfo {
public:

	int winner;

	gmOutputDealEndInfo& operator=(const gmOutputDealEndInfo& value) {
		winner = value.winner;

		return(*this);
	}
};

#endif
