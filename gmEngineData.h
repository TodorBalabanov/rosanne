#ifndef GMENGINEDATA_H_
#define GMENGINEDATA_H_

#include "Constants.h"
#include "gmTrick.h"
#include "gmRules.h"
#include "gmOutputDealInfo.h"
#include "gmOutputDealEndInfo.h"
#include "gmInputBidInfo.h"
#include "gmInputTrumpselInfo.h"
#include "gmInputTrickInfo.h"

class gmEngineData {
public:

	bool ok;
	bool feedback;

	gmRules rules;

	int status;
	int dealer;

	int shuffled[gmTOTAL_CARDS];

	// Variables related to messaging
	bool output_pending;
	bool input_pending;

	int output_type;
	int input_type;

	// Related to bidding
	bool first_bid;
	int curr_max_bid;
	int curr_max_bidder;
	int last_bidder;
	bool bid_hist[gmTOTAL_BID_ROUNDS][gmTOTAL_PLAYERS];
	unsigned long passed_round1;

	// Related to the trump card
	int trump_suit;
	int trump_card;

	// Cards held by players
	unsigned long hands[gmTOTAL_PLAYERS];

	// Related to tricks
	int pts[gmTOTAL_TEAMS];
	int trick_round;
	gmTrick tricks[gmTOTAL_TRICKS];
	unsigned long played_cards[gmTOTAL_PLAYERS];
	bool should_trump;
	bool should_play_trump_card;
	bool trump_shown;

	// Related to output messages
	gmOutputDealInfo out_deal_info;
	gmOutputDealEndInfo out_deal_end_info;

	// Structures to hold data related to the pending input message
	gmInputBidInfo in_bid_info;
	gmInputTrumpselInfo in_trumpsel_info;
	gmInputTrickInfo in_trick_info;

	gmEngineData& operator=(const gmEngineData& value) {
		ok = value.ok;
		feedback = value.feedback;
		rules = value.rules;
		status = value.status;
		dealer = value.dealer;

		for(int i=0; i<gmTOTAL_CARDS; i++) {
			shuffled[i] = value.shuffled[i];
		}

		output_pending = value.output_pending;
		input_pending = value.input_pending;
		output_type = value.output_type;
		input_type = value.input_type;
		first_bid = value.first_bid;
		curr_max_bid = value.curr_max_bid;
		curr_max_bidder = value.curr_max_bidder;
		last_bidder = value.last_bidder;

		for(int i=0; i<gmTOTAL_BID_ROUNDS; i++) {
			for(int j=0; j<gmTOTAL_PLAYERS; j++) {
				bid_hist[i][j] = value.bid_hist[i][j];
			}
		}

		passed_round1 = value.passed_round1;
		trump_suit = value.trump_suit;
		trump_card = value.trump_card;

		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			hands[i] = value.hands[i];
		}

		for(int i=0; i<gmTOTAL_TEAMS; i++) {
			pts[i] = value.pts[i];
		}

		trick_round = value.trick_round;

		for(int i=0; i<gmTOTAL_TRICKS; i++) {
			tricks[i] = value.tricks[i];
		}

		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			played_cards[i] = value.played_cards[i];
		}

		should_trump = value.should_trump;
		should_play_trump_card = value.should_play_trump_card;
		trump_shown = value.trump_shown;
		out_deal_info = value.out_deal_info;
		out_deal_end_info = value.out_deal_end_info;
		in_bid_info = value.in_bid_info;
		in_trumpsel_info = value.in_trumpsel_info;
		in_trick_info = value.in_trick_info;

		return(*this);
	}
};


#endif
