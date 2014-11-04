// Rosanne : Trump card game popularly known as Twenty Eight (28)
// Copyright (C) 2006-2010 Vipin Cherian
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef _GMENGINE_H_
#define _GMENGINE_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "gmutil.h"

enum {
	gmSTATUS_NOT_STARTED = 0,
	gmSTATUS_DEAL1,
	gmSTATUS_BID1,
	gmSTATUS_BID2,
	gmSTATUS_TRUMPSEL1,
	gmSTATUS_DEAL2,
	gmSTATUS_BID3,
	gmSTATUS_TRUMPSEL2,
	gmSTATUS_TRICKS,
	gmSTATUS_FINISHED
};

enum {
	gmOUTPUT_INVALID = -1,
	gmOUTPUT_STARTED,
	gmOUTPUT_DEAL,
	gmOUPUT_BID1,
	gmOUPUT_BID2,
	gmOUTPUT_TRUMPSEL,
	gmOUTPUT_TRICK,
	gmOUTPUT_DEAL_END
};

enum {
	gmINPUT_INVALID = -1,
	gmINPUT_BID,
	gmINPUT_TRUMPSEL,
	gmINPUT_TRICK
};
enum {
	gmERR_CANNOT_PASS = 1,
	gmERR_BID_LESS_THAN_MIN,
	gmERR_BID_BY_WRONG_PLAYER,
	gmERR_TRUMPSEL_BY_WRONG_PLAYER,
	gmERR_TRUMPSEL_NONEXIST_CARD,
	gmERR_TRICK_BY_WRONG_PLAYER,
	gmERR_TRICK_INVALID_TRUMP_REQ,
	gmERR_TRICK_MASK_MISMATCH,
	gmERR_TRICK_CARD_NOT_IN_HAND
};

#define	gmRULE_1 1
#define	gmRULE_2 2
#define	gmRULE_3 4
#define	gmRULE_4 8
// Sluffing of jacks
#define	gmRULE_5 16

#define gmDEAL_ROUND_1 0
#define gmDEAL_ROUND_2 1

#define raBID_ROUND_3 2

#define gmFOUR_JACKS 0x80808080
#define gmJACK 0x80
#define gmALL_CARDS 0xFFFFFFFF

class gmRules {
public:
	int rot_addn;
	int min_bid_1;
	int min_bid_2;
	int min_bid_3;
	bool waive_rule_4;
	bool sluff_jacks;

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

typedef gmRules* pgmRules;

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

#define gmNext(X) ((X + m_data.rules.rot_addn) % gmTOTAL_PLAYERS)
#define gmTrickNext ((m_data.tricks[m_data.trick_round].lead_loc + (m_data.tricks[m_data.trick_round].count * m_data.rules.rot_addn)) % 4)
#define gmWinnerCard (m_data.tricks[m_data.trick_round].cards[m_data.tricks[m_data.trick_round].winner])

class gmOutputDealInfo {
public:

	int round;
	unsigned long hands[gmTOTAL_PLAYERS];

	gmOutputDealInfo& operator=(const gmOutputDealInfo& value) {
		round = value.round;

		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			hands[i] = value.hands[i];
		}

		return(*this);
	}
};

class gmOutputTrickInfo {
public:
	int points[gmTOTAL_TEAMS];
	gmTrick trick;
} ;

class gmOutputDealEndInfo {
public:

	int winner;

	gmOutputDealEndInfo& operator=(const gmOutputDealEndInfo& value) {
		winner = value.winner;

		return(*this);
	}
};

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

class gmEngine {
private:
	gmEngineData m_data;
	void SetOutput(int output_type);
	void SetInput(int input_type);
	unsigned long GenerateMask(unsigned long *rules = NULL);
	bool SetDealEndOutput();
	// Disallow copy constructor/assignment operators
	gmEngine(const gmEngine &);

public:
	// Initialization of the gmEngineData structure used by each gmEngine instance (m_data) is a costly affair
	// because 1) the structure is huge 2) there are multiple loops to be run to initiate certian data elements.
	// At one point of time, approx 30% of the entire run time was taken by gmEntine.Reset. To speed things up,
	// a static m_init of type gmEngineData is created and is initiated manually when the first instance of gmEngine is
	// called. Once this is done, all further initializations of this struct is done be a memory copy from m_init.
	static gmEngineData m_init;
	static void InitCache();
	static bool m_init_ok;

	gmEngine();
	virtual ~gmEngine();
	bool IsOk();
	int GetStatus();
	static bool Reset(gmEngineData *data);
	bool Reset();
	bool Shuffle();
	bool Continue();
	bool GetOutput(int *output_type, void *output);
	bool IsOutputPending();
	int GetPendingOutputType();
	bool IsInputPending();
	int GetPendingInputType();
	bool GetPendingInputCriteria(int *input_type, void *input);
	int PostInputMessage(int input_type, void *input);
	bool GetFeedback();
	void SetFeedback(bool feedback);
	void GetRules(gmRules *rules);
	void SetRules(gmRules *rules);
	void GetHands(unsigned long *hands);
	void GetCardsPlayed(unsigned long *cards);
	// TODO : Try to implement one method with trick_round as default variable
	void GetTrick(int trick_round, gmTrick *trick);
	void GetTrick(gmTrick *trick);
	int GetTrickRound();
	int GetPoints(int team);
	void GetPoints(int *pts);
	int GetTrump();
	int GetTrumpCard();
	int GetDealer();
	void SetDealer(int dealer);
	static void ResetTrick(gmTrick *trick);
	bool GetData(gmEngineData *data);
	bool SetData(gmEngineData *data, bool check = true);
	bool GetMaxBid(int *bid, int *loc);
	wxString GetLoggable();
	static wxString PrintRuleEngineData(gmEngineData *data);
	bool IsTrumpShown();
	int GetTrickNextToPlay();
	void SetMinBid(int round, int bid);
	void SetWaiveRuleFour(bool flag);
	void SetSluffJacks(bool flag);
};



#endif
