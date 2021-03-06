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

#include "gmengine.h"

//#define gmREAD_DEAL_FROM_FILE 0

#ifdef gmREAD_DEAL_FROM_FILE
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#endif

//
// Constructor
//

bool gmEngine::m_init_ok = false;
gmEngineData gmEngine::m_init;

gmEngine::gmEngine() {
	if(m_init_ok == false) {
		gmEngine::InitCache();
		m_init_ok = true;
	}
	m_data.ok = Reset(&m_data);
	m_data.feedback = true;

	// Set the rules
	m_data.rules.rot_addn = 1;
	m_data.rules.min_bid_1 = 14;
	m_data.rules.min_bid_2 = 20;
	m_data.rules.min_bid_3 = 23;
	m_data.rules.waive_rule_4 = false;
	m_data.rules.sluff_jacks = true;
}

//
// Destructor
//

gmEngine::~gmEngine() {
}

//
// Public Methods
//

bool gmEngine::IsOk() {
	return m_data.ok;
}
int gmEngine::GetStatus() {
	return m_data.status;
}
bool gmEngine::Reset(gmEngineData *data) {
	bool feedback;
	gmRules rules;
	feedback = data->feedback;
	rules = data->rules;
	*data = gmEngine::m_init;
	data->feedback = feedback;
	data->rules = rules;
	return true;
}

void gmEngine::InitCache() {
	m_init.ok=false;
	m_init.feedback=false;
	m_init.rules.min_bid_1=0;
	m_init.rules.min_bid_2=0;
	m_init.rules.min_bid_3=0;
	m_init.rules.sluff_jacks=false;
	m_init.rules.waive_rule_4=false;
	m_init.status=0;
	m_init.dealer=0;
	for (int i=0; i<gmTOTAL_CARDS;i++){
		m_init.shuffled[i]=0;
	}
	m_init.output_pending=false;
	m_init.input_pending=false;
	m_init.input_type=0;
	m_init.output_type=0;
	m_init.first_bid=false;
	m_init.curr_max_bid=0;
	m_init.curr_max_bidder=0;
	m_init.last_bidder=0;
	for (int i=0; i<gmTOTAL_BID_ROUNDS;i++){
		for(int j=0; j<gmTOTAL_PLAYERS;j++){
			m_init.bid_hist [i][j]=false;
		}
	}
	m_init.passed_round1 = 0;
	m_init.trump_suit = 0;
	m_init.trump_card = 0;
	for (int i = 0;i<gmTOTAL_PLAYERS;i++){
		m_init.hands[i] = 0;
	}
	for (int i= 0;i<gmTOTAL_TEAMS;i++){
		m_init.pts[i]=0;
	}
	m_init.trick_round = 0;
	for (int i= 0;i<gmTOTAL_TRICKS;i++){
		m_init.tricks[i].trumped=false;
		for(int j = 0; j < gmTOTAL_PLAYERS;j++){
			m_init.tricks[i].cards[j] = 0;
		}
		m_init.tricks[i].lead_suit = 0;
		m_init.tricks[i].lead_loc = 0;
		m_init.tricks[i].count = 0;
		m_init.tricks[i].points = 0;
		m_init.tricks[i].winner = 0;
	}
	for (int i=0; i<gmTOTAL_PLAYERS; i++){
		m_init.played_cards[i]=0;
	}
	m_init.should_trump= false;
	m_init.should_play_trump_card= false;
	m_init.trump_shown= false;
	m_init.out_deal_info = (short)0;
	m_init.out_deal_end_info.winner=0;
	m_init.in_bid_info.player=0;
	m_init.in_bid_info.min=0;
	m_init.in_bid_info.passable=false;
	m_init.in_bid_info.bid=0;
	m_init.in_bid_info.round=0;
	m_init.in_trumpsel_info.card=0;
	m_init.in_trumpsel_info.player=0;
	m_init.in_trick_info.player=0;
	m_init.in_trick_info.can_ask_trump=false;
	m_init.in_trick_info.ask_trump=false;
	m_init.in_trick_info.mask=0;
	m_init.in_trick_info.card=0;
	m_init.in_trick_info.rules=0;
	//TODO : Reduce the number of loops

	gmEngine::m_init.status = gmSTATUS_NOT_STARTED;
	gmEngine::m_init.dealer = 0;

	// Filling m_shuffled with values for all the 32 cards
	for(int i = 0; i < gmTOTAL_CARDS; i++)
		gmEngine::m_init.shuffled[i] = i;

	// Neither input or output is pending at the start
	gmEngine::m_init.input_pending = false;
	gmEngine::m_init.output_pending = false;

	// Reset the hands
	for(int i = 0; i < gmTOTAL_PLAYERS; i++)
		gmEngine::m_init.hands[i] = 0;

	// Resetting variables related to bidding
	gmEngine::m_init.first_bid = true;
	gmEngine::m_init.curr_max_bid = 0;
	gmEngine::m_init.curr_max_bidder = gmPLAYER_INVALID;
	gmEngine::m_init.last_bidder = gmPLAYER_INVALID;
	for(int i = 0; i < gmTOTAL_BID_ROUNDS; i++)
		for(int j = 0; j < gmTOTAL_PLAYERS; j++)
			gmEngine::m_init.bid_hist[i][j] = false;
	gmEngine::m_init.passed_round1 = 0;

	// Resetting trump suit and card
	gmEngine::m_init.trump_card = gmCARD_INVALID;
	gmEngine::m_init.trump_suit = gmSUIT_INVALID;

	// Resetting variables related to tricks
	for(int i = 0; i < gmTOTAL_TEAMS; i++)
		gmEngine::m_init.pts[i] = 0;

	for(int i = 0; i < gmTOTAL_PLAYERS; i++)
		gmEngine::m_init.played_cards[i] = 0;

	gmEngine::m_init.trick_round = 0;
	gmEngine::m_init.should_trump = false;
	gmEngine::m_init.should_play_trump_card = false;
	gmEngine::m_init.trump_shown = false;

	// TODO : Use ResetTrick
	for(int i = 0; i < gmTOTAL_TRICKS; i++) {
		for(int j = 0; j < gmTOTAL_PLAYERS; j++)
			gmEngine::m_init.tricks[i].cards[j] = gmCARD_INVALID;
		gmEngine::m_init.tricks[i].count = 0;
		gmEngine::m_init.tricks[i].lead_loc = gmPLAYER_INVALID;
		gmEngine::m_init.tricks[i].lead_suit = gmSUIT_INVALID;
		gmEngine::m_init.tricks[i].points = 0;
		gmEngine::m_init.tricks[i].trumped = false;
		m_init.tricks[i].winner = gmPLAYER_INVALID;
	}

//    gmEngine::m_init.feedback = true;
//	gmEngine::m_init.rules.rot_addn = 1;
//	gmEngine::m_init.rules.min_bid_1 = 14;
//	gmEngine::m_init.rules.min_bid_2 = 20;
//	gmEngine::m_init.rules.min_bid_3 = 23;
//	gmEngine::m_init.rules.waive_rule_4 = false;
//	gmEngine::m_init.rules.sluff_jacks = true;

	return;

}
bool gmEngine::Reset() {
	return Reset(&m_data);
}
bool gmEngine::Shuffle() {
	int i = 0;

	// If required set the shuffled card as per the
	// deal read from the test data input file
#ifdef gmREAD_DEAL_FROM_FILE

	if(::wxFileExists(raTEST_DATA_FILE)) {
		wxString str_cards_read;
		wxString text_round;
		wxString key;
		wxString str_card;
		int idx;
		wxFFileInputStream in(raTEST_DATA_FILE);
		wxFileConfig fcfg(in);
		int k, l;
		unsigned long cards_read[2][gmTOTAL_PLAYERS];
		unsigned long all_read = 0;
		unsigned long temp;
		int count_read = 0;

		wxLogDebug("----------------------------------------------");
		wxLogDebug("Attempting to read deal information from file");

		for(i = 0; i < 2; i++)
			for(j = 0; j < gmTOTAL_PLAYERS; j++)
				cards_read[i][j] = 0;

		for(k = 1; k <= 2; k++) {
			text_round = wxString::Format("%s%d", raTEXT_DEAL_ROUND, k);
			if(!fcfg.Exists(text_round))
				continue;

			for(i = 0; i < gmTOTAL_PLAYERS; i++) {
				key = wxString::Format("%s/%s", text_round.c_str(), gmUtil::m_short_locs[i].c_str());
				if(fcfg.Exists(key)) {
					if(!fcfg.Read(key, &str_cards_read)) {
						wxLogError(wxString::Format(
									   wxT("Read failed. %s:%d"), wxT(__FILE__), __LINE__));

					} else {
						wxLogDebug(wxString::Format(
									   "Cards to be dealt to %s are %s",
									   gmUtil::m_short_locs[i].c_str(), str_cards_read.c_str()));

						// Get each str_card from the list of str_cards to be
						// dealt to the location
						while(!str_cards_read.IsEmpty()) {
							j = str_cards_read.Find(',');
							str_card = str_cards_read.Left(j);
							str_card.UpperCase();
							str_card.Trim();
							str_card.Trim(false);
							wxLogDebug(wxString::Format("Card %s", str_card.c_str()));
							idx = gmUtil::GetCardIndex(str_card);
							if(idx == -1) {
								wxLogDebug(wxString::Format(
											   wxT("GetCardIndex failed. %s:%d"),
											   wxT(__FILE__), __LINE__));
								break;
							} else {
								cards_read[k - 1][i] |= (((unsigned long)1) << idx);
								count_read++;
							}

							if(j == -1)
								break;
							str_cards_read = str_cards_read.Mid(j + 1);
						}
						wxASSERT(gmUtil::CountBitsSet(cards_read[k - 1][i]) <= 4);
					}
				}
			}
			wxLogDebug(wxString::Format("For round %d", k));
			// Print the cards to be dealt for each player
			for(i = 0; i < gmTOTAL_PLAYERS; i++) {
				wxLogDebug(wxString::Format("Cards for %s - %s",
											gmUtil::m_short_locs[i].c_str(),
											gmUtil::PrintLong(cards_read[k - 1][i]).c_str()));
			}
		}
		/*else
		{
		wxLogDebug(wxString::Format(
		wxT("Cards to be dealt for round 1 not read from %s. %s:%d"),
		raTEST_DATA_FILE, wxT(__FILE__), __LINE__));
		}*/
		for(i = 0; i < 2; i++)
			for(j = 0; j < gmTOTAL_PLAYERS; j++)
				all_read |= cards_read[i][j];
		if((int)gmUtil::CountBitsSet(all_read) != count_read) {
			wxLogError(wxString::Format(
						   "Duplicate cards. From all read %d count read %d",
						   (int)gmUtil::CountBitsSet(all_read), count_read));
		} else {
			int * unassigned = new int[gmTOTAL_CARDS - count_read];
			j = 0;
			for(i = 0; i < gmTOTAL_CARDS; i++) {
				if(!(all_read & (((unsigned long)1) << i)))
					unassigned[j++] = i;
			}
			wxASSERT(j == (gmTOTAL_CARDS - count_read));
			gmUtil::ShuffleArray(unassigned, (gmTOTAL_CARDS - count_read));

			bool flags[gmTOTAL_CARDS];
			for(i = 0; i < gmTOTAL_CARDS; i++) {
				flags[i] = false;
			}
			for(k = 0; k < 2; k++) {
				for(i = 0; i < gmTOTAL_PLAYERS; i++) {
					j = 0;
					temp = cards_read[k][i];
					while(temp) {
						l = (k * (gmTOTAL_CARDS / 2)) + (i * (gmTOTAL_CARDS / (gmTOTAL_PLAYERS * 2))) +  j;
						j++;
						m_data.shuffled[l] = gmUtil::HighestBitSet(temp);
						flags[l] = true;
						temp &= ~(((unsigned long)1) << m_data.shuffled[l]);
						//temp &= ~(((unsigned long)1) << gmUtil::HighestBitSet(temp));
					}
					wxASSERT(j == (int)(gmUtil::CountBitsSet(cards_read[k][i])));
				}
			}
			j = 0;
			for(i = 0; i < gmTOTAL_CARDS; i++) {
				if(!flags[i]) {
					m_data.shuffled[i] = unassigned[j++];
					flags[i] = true;
				}
			}
			for(i = 0; i < gmTOTAL_CARDS; i++) {
				wxASSERT(flags[i]);
				//wxLogDebug(wxString::Format("%d - %s%s",
				//	i,
				//	gmUtil::m_suits[gmGetSuit(m_data.shuffled[i])],
				//	gmUtil::m_values[gmGetValue(m_data.shuffled[i])]
				//	));
			}

			delete unassigned;
			wxLogDebug("-------------Succes---------------------------");
			return true;
		}
		wxLogDebug("----------------------------------------------");
	}
#endif

	// Sorting, for full replication
	// can be removed, functionality will not be affected.
	for (i = 0; i < gmTOTAL_CARDS; i++)
		m_data.shuffled[i] = i;

	gmUtil::ShuffleArray(m_data.shuffled, gmTOTAL_CARDS);

	return true;
}
bool gmEngine::Continue() {
	int i;
	unsigned long u;
	unsigned long rules;

	// Cannot continue if input is pending
	if(m_data.input_pending)
		return false;

	// Cannot continue if output is pending
	if(m_data.output_pending)
		return false;

	switch(m_data.status) {

	case gmSTATUS_NOT_STARTED:
		if(m_data.feedback)
			SetOutput(gmOUTPUT_STARTED);
		m_data.status++;
		return false;
		break;

	case gmSTATUS_DEAL1:
		// Shuffle the cards before dealing round 1
		if(!Shuffle()) {
			wxLogDebug(wxString::Format(
						   wxT("Unexpected error while shuffling cards. File - %s Line - %d"),
						   wxT(__FILE__), __LINE__));
			return false;
		}

		// Deal round 1
		for(i = 0; i < gmTOTAL_CARDS / 2; i++) {
			m_data.hands[i / 4] |= (((unsigned long)1) << m_data.shuffled[i]);
		}

		if(m_data.feedback) {
			m_data.out_deal_info.round = gmDEAL_ROUND_1;
			//TODO : Is this memory copy correct?
			for(int i=0; i<gmTOTAL_PLAYERS; i++) {
				m_data.out_deal_info.hands[i] = m_data.hands[i];
			}

			// Set output pending
			SetOutput(gmOUTPUT_DEAL);
		}
		m_data.status++;
		return false;
		break;

	case gmSTATUS_BID1:
		// If All Tricks has already been bid,
		// move to next stage
		if(m_data.curr_max_bid == gmBID_ALL) {
			m_data.status++;
			return false;
		}
		// If no bids have been made yet, start with the player next to dealer
		if(m_data.last_bidder == gmPLAYER_INVALID)
			i = gmNext(m_data.dealer);
		// Otherwise, start with the player next to the last bidder
		else
			i = gmNext(m_data.last_bidder);

		// u will hold the list of players for which
		// it was checked as to whether the player can bid
		u = 0;
		while(u < 0x0000000F) {
			// If the player has not already bid
			// and the highest bid is not held by his partner
			// then a bid is expected.
			if(!m_data.bid_hist[0][i] && (gmPartner(i)
										  != m_data.curr_max_bidder)) {
				// Fill data in the input bid structure
				m_data.in_bid_info.player = i;
				m_data.in_bid_info.bid = 0;

				if(m_data.last_bidder == gmPLAYER_INVALID)
					m_data.in_bid_info.passable = false;
				else
					m_data.in_bid_info.passable = true;

				// If no bid has been made yet, the minimum
				// that can be bid is the minimum for the first round.
				// Otherwise the minimum bid that can be made
				// is any bid greater than the highest bid made so far.
				if(m_data.curr_max_bid == 0)
					m_data.in_bid_info.min = m_data.rules.min_bid_1;
				else
					m_data.in_bid_info.min = m_data.curr_max_bid + 1;

				m_data.in_bid_info.round = 0;

				SetInput(gmINPUT_BID);

				return false;
			}
			u |= (((unsigned long)1) << i);
			i = gmNext(i);
		}
		// Before moving to round 2 of the auction,
		// reset values
		m_data.last_bidder = gmPLAYER_INVALID;

		m_data.status++;
		return false;
		break;

	case gmSTATUS_BID2:
		// If All Tricks has already been bid,
		// move to next stage
		if(m_data.curr_max_bid == gmBID_ALL) {
			m_data.status++;
			return false;
		}
		// If no bids have been made yet in this round,
		// start with the player next to dealer
		if(m_data.last_bidder == gmPLAYER_INVALID)
			i = gmNext(m_data.dealer);
		// Otherwise, start with the player next to the last bidder
		else
			i = gmNext(m_data.last_bidder);

		// u will hold the list of players for which
		// it was checked as to whether the player can bid
		u = 0;
		while(u < 0x0000000F) {
			// If the player has not already bid
			// and the highest bid is not by the player
			// then a bid is expected and if the player
			// had not passed in the first round
			if(
				!m_data.bid_hist[1][i] &&
				(i != m_data.curr_max_bidder) &&
				!(m_data.passed_round1 & (((unsigned long)1) << i))
			) {
				// Fill data in the input bid structure
				m_data.in_bid_info.player = i;
				m_data.in_bid_info.bid = 0;

				m_data.in_bid_info.passable = true;

				// The minimum bid that can be made is the
				// maximum of the current highest bid and the minimum allowed
				// bid for the round.
				m_data.in_bid_info.min =
					std::max(m_data.rules.min_bid_2, (m_data.curr_max_bid + 1));
				m_data.in_bid_info.round = 1;

				SetInput(gmINPUT_BID);

				return false;
			}
			u |= (((unsigned long)1) << i);
			i = gmNext(i);
		}
		// Before moving to round 3 of the auction,
		// reset values
		m_data.last_bidder = gmPLAYER_INVALID;

		m_data.status++;
		return false;
		break;

	case gmSTATUS_BID3:
		// If All Tricks has already been bid,
		// move to next stage
		if(m_data.curr_max_bid == gmBID_ALL) {
			m_data.status++;
			return false;
		}
		// If no bids have been made yet in this round,
		// start with the player next to dealer
		if(m_data.last_bidder == gmPLAYER_INVALID)
			i = gmNext(m_data.dealer);
		// Otherwise, start with the player next to the last bidder
		else
			i = gmNext(m_data.last_bidder);

		// u will hold the list of players for which
		// it was checked as to whether the player can bid
		u = 0;
		while(u < 0x0000000F) {
			// If the player has not already bid
			// then a bid is expected.
			if(!m_data.bid_hist[2][i]) {
				// Fill data in the input bid structure
				m_data.in_bid_info.player = i;
				m_data.in_bid_info.bid = 0;

				m_data.in_bid_info.passable = true;

				// The minimum bid that can be made is the
				// maximum of the current highest bid and the minimum allowed
				// bid for the round.
				m_data.in_bid_info.min =
					std::max(m_data.rules.min_bid_3, (m_data.curr_max_bid + 1));
				m_data.in_bid_info.round = 2;

				SetInput(gmINPUT_BID);

				return false;
			}
			u |= (((unsigned long)1) << i);
			i = gmNext(i);
		}
		// Before moving to round 3 of the auction,
		// reset values
		m_data.last_bidder = gmPLAYER_INVALID;

		m_data.status++;
		return false;
		break;

	case gmSTATUS_TRUMPSEL1:
	case gmSTATUS_TRUMPSEL2:
		if((m_data.trump_card == gmCARD_INVALID) && (m_data.curr_max_bid != gmBID_ALL)) {
			// Fill data in the input trumpsel info structure
			m_data.in_trumpsel_info.card = gmCARD_INVALID;
			m_data.in_trumpsel_info.player = m_data.curr_max_bidder;
			SetInput(gmINPUT_TRUMPSEL);
		}

		// Set the player to lead the first trick
		m_data.tricks[0].lead_loc = gmNext(m_data.dealer);

		m_data.status++;
		break;

	case gmSTATUS_DEAL2:
		// Deal round 1
		for(i = 0; i < gmTOTAL_CARDS / 2; i++) {
			m_data.hands[i / 4] |=
				(((unsigned long)1) << m_data.shuffled[i + (gmTOTAL_CARDS / 2)]);
		}

		if(m_data.feedback) {
			m_data.out_deal_info.round = gmDEAL_ROUND_2;
			// TODO : Is this memory copy correct?
			for(int i=0; i<gmTOTAL_PLAYERS; i++) {
				m_data.out_deal_info.hands[i] = m_data.hands[i];
			}

			// Set output pending
			SetOutput(gmOUTPUT_DEAL);
		}
		m_data.status++;
		return false;
		break;
	case gmSTATUS_TRICKS:
		if(m_data.trick_round < gmTOTAL_TRICKS) {

			m_data.in_trick_info.ask_trump = false;

			// A player can ask for the trump
			// only if the bid is not for All tricks
			if(m_data.curr_max_bid != gmBID_ALL) {
				// Player can ask for trump to be shown if,
				// 1. Trump is not shown
				// 2. The trick has a valid lead suit
				// 3. Player does not have any lead suit
				m_data.in_trick_info.can_ask_trump = false;
				if(
					!m_data.trump_shown &&
					(m_data.tricks[m_data.trick_round].lead_suit != gmSUIT_INVALID)
				) {
					if(
						!(m_data.hands[gmTrickNext] &
						  gmUtil::m_suit_mask[m_data.tricks[m_data.trick_round].lead_suit])
					) {
						m_data.in_trick_info.can_ask_trump = true;
					}
				}

				// Player can also ask for trump if
				// 1. He is the max bidder
				// 2. And trump is not shown
				// 3. and he does not have any card in his hand

				if(
					(gmTrickNext == m_data.curr_max_bidder) &&
					!m_data.trump_shown &&
					!m_data.hands[gmTrickNext]
				)
					m_data.in_trick_info.can_ask_trump = true;
			}

			m_data.in_trick_info.card = gmCARD_INVALID;

			// Obtain the mask and the rules applicable
			// and set the same
			rules = 0;
			m_data.in_trick_info.mask = GenerateMask(&rules);
			m_data.in_trick_info.rules = rules;
			m_data.in_trick_info.player = gmTrickNext;

			// If the player has no card which matches
			// the mask, player can play any card
			if(!(m_data.hands[m_data.in_trick_info.player] & m_data.in_trick_info.mask)) {
				// If jacks cannot be sluffed, set the mask appropriately
				if(!m_data.rules.sluff_jacks) {
					unsigned long jacks = gmFOUR_JACKS;
					// Rule 5 is applicable only if the player is not leading
					if(m_data.tricks[m_data.trick_round].count) {
						// If trump is shown the Jack of trumps can be played
						if(m_data.trump_shown) {
							jacks &= ~((unsigned long)gmJACK << gmUtil::m_suit_rs[m_data.trump_suit]);
						}
						m_data.in_trick_info.mask = gmALL_CARDS & ~(jacks);
						m_data.in_trick_info.rules = gmRULE_5;

					}
					// If after applying rule 5, no cards can be played
					// reset the mask
					if(!(m_data.hands[m_data.in_trick_info.player] & m_data.in_trick_info.mask))
						m_data.in_trick_info.mask = gmALL_CARDS;
				} else
					m_data.in_trick_info.mask = gmALL_CARDS;
			}

			wxASSERT(m_data.in_trick_info.mask);

			SetInput(gmINPUT_TRICK);
			return false;
		}
		m_data.status++;
		break;
	case gmSTATUS_FINISHED:
		SetDealEndOutput();
		return true;
		break;
	}

	return false;
}


bool gmEngine::GetOutput(int *output_type, void *output) {
	// If output is not pending, return false
	if(!m_data.output_pending)
		return false;

	if(output_type)
		*(int*)output_type = m_data.output_type;
	switch(m_data.output_type) {
	case gmOUTPUT_STARTED:
		break;
	case gmOUTPUT_DEAL:
		*(gmOutputDealInfo*)output = m_data.out_deal_info;
		break;
	case gmOUTPUT_DEAL_END:
		*(gmOutputDealEndInfo*)output = m_data.out_deal_end_info;
		break;
	default:
		wxLogDebug(wxString::Format(wxT("Inside default in switch. File - %s Line - %d"), wxT(__FILE__), __LINE__));
		break;
	}

	m_data.output_pending = false;
	return true;
}
bool gmEngine::IsOutputPending() {
	return m_data.output_pending;
}
int gmEngine::GetPendingOutputType() {
	// If no output is pending,
	// return invalid type
	if(!m_data.output_pending)
		return gmOUTPUT_INVALID;

	return m_data.output_type;
}

bool gmEngine::IsInputPending() {
	return m_data.input_pending;
}
int gmEngine::GetPendingInputType() {
	// If no input is pending,
	// return invalid type
	if(!m_data.input_pending)
		return gmINPUT_INVALID;

	return m_data.input_type;
}

bool gmEngine::GetPendingInputCriteria(int *input_type, void *input) {
	// If input is not pending, return false
	if(!m_data.input_pending)
		return false;

	if(input_type)
		*input_type = m_data.input_type;

	switch(m_data.input_type) {
	case gmINPUT_BID:
		*(gmInputBidInfo*)input = m_data.in_bid_info;
		break;
	case gmINPUT_TRUMPSEL:
		*(gmInputTrumpselInfo*)input = m_data.in_trumpsel_info;
		break;
	case gmINPUT_TRICK:
		*(gmInputTrickInfo*)input = m_data.in_trick_info;
		break;
	default:
		wxLogDebug(wxString::Format(wxT("Inside default in switch. File - %s Line - %d"), wxT(__FILE__), __LINE__));
		break;
	}

	return true;
}
int gmEngine::PostInputMessage(int input_type, void *input) {
	gmInputBidInfo *in_bid_info, *exist_bid_info;
	gmInputTrumpselInfo *in_trumpsel_info, *exist_trumpsel_info;
	gmInputTrickInfo *in_trick_info, *exist_trick_info;
	// Check whether input is pending
	if(!m_data.input_pending)
		return false;

	// Check whether the input types is correct
	if(input_type != m_data.input_type)
		return false;

	switch(m_data.input_type) {
	case gmINPUT_BID:
		in_bid_info = (gmInputBidInfo *)input;
		exist_bid_info = &m_data.in_bid_info;

		// Check whether the player making the bid is correct
		if(exist_bid_info->player != in_bid_info->player)
			return gmERR_BID_BY_WRONG_PLAYER;

		// If passed, check whether the bid is indeed passable
		if(in_bid_info->bid == gmBID_PASS) {
			if(!exist_bid_info->passable)
				return gmERR_CANNOT_PASS;

			// If pass is a valid bid, accept the bid
			m_data.last_bidder = exist_bid_info->player;
			m_data.bid_hist[exist_bid_info->round][exist_bid_info->player] = true;
			// If a player is passing in round 1, keep a note of the same,
			// so that the player cannot bid in round 2
			if(exist_bid_info->round == 0)
				m_data.passed_round1 |= (((unsigned long)1) << exist_bid_info->player);
			break;
		}

		// Check whether the bid is less than the minimum allowed
		if(in_bid_info->bid < exist_bid_info->min)
			return gmERR_BID_LESS_THAN_MIN;

		// If there is an existing trump, invalidate the same
		// and add the card back to the max bidder
		if(m_data.trump_card != gmCARD_INVALID) {
			m_data.hands[m_data.curr_max_bidder] |= (((unsigned long)1) << m_data.trump_card);
			m_data.trump_card = gmCARD_INVALID;
			m_data.trump_suit = gmSUIT_INVALID;
		}

		// If the bid is valid, accept the same
		m_data.first_bid = false;
		m_data.curr_max_bid = in_bid_info->bid;
		m_data.curr_max_bidder = exist_bid_info->player;
		m_data.last_bidder = exist_bid_info->player;
		m_data.bid_hist[exist_bid_info->round][exist_bid_info->player] = true;


		break;
	case gmINPUT_TRUMPSEL:
		in_trumpsel_info = (gmInputTrumpselInfo *)input;
		exist_trumpsel_info = &m_data.in_trumpsel_info;

		// Verify that the player is correct
		if(in_trumpsel_info->player != exist_trumpsel_info->player)
			return gmERR_TRUMPSEL_BY_WRONG_PLAYER;

		// Verify that the trump card has a valid value
		wxASSERT(in_trumpsel_info->card >= 0);
		wxASSERT(in_trumpsel_info->card < gmTOTAL_CARDS);

		// Verify that the card exists in the highest bidders hand
		if(!(m_data.hands[exist_trumpsel_info->player] & (((unsigned long)1) << in_trumpsel_info->card)))
			return gmERR_TRUMPSEL_NONEXIST_CARD;

		// Set the trump card
		m_data.trump_suit = in_trumpsel_info->card / gmTOTAL_VALUES;
		m_data.trump_card = in_trumpsel_info->card;

		// TODO : Do this earlier, when the bid is made
		// Remove the trump card from the highest bidders hand
		m_data.hands[exist_trumpsel_info->player] &= ~(((unsigned long)1) << m_data.trump_card);

		// As soon as a trump is selected, the status can be incremented
		//m_data.status++;

		break;
	case gmINPUT_TRICK:
		in_trick_info = (gmInputTrickInfo *)input;
		exist_trick_info = &m_data.in_trick_info;

		// Verify that the player is correct
		if(in_trick_info->player != exist_trick_info->player)
			return gmERR_TRICK_BY_WRONG_PLAYER;

		// Is the player asking for trump to be shown?
		if(in_trick_info->ask_trump) {
			if(!exist_trick_info->can_ask_trump)
				return gmERR_TRICK_INVALID_TRUMP_REQ;

			m_data.trump_shown = true;
			m_data.should_trump = true;

			// Add the trump card to max bidder's hand
			m_data.hands[m_data.curr_max_bidder] |= (((unsigned long)1) << m_data.trump_card);

			if(exist_trick_info->player == m_data.curr_max_bidder) {
				m_data.should_play_trump_card = true;
				m_data.in_trick_info.mask = (((unsigned long)1) << m_data.trump_card);
			} else {
				// Check if the player who asked for trump has
				// at least a single trump. If that is the case,
				// then he should play trump. Set mask accordingly
				if(m_data.hands[m_data.in_trick_info.player] & gmUtil::m_suit_mask[m_data.trump_suit]) {
					m_data.in_trick_info.mask &= gmUtil::m_suit_mask[m_data.trump_suit];
				}
			}

			break;
		}

		// If the player has played a card
		wxASSERT(in_trick_info->card >= 0);
		wxASSERT(in_trick_info->card < gmTOTAL_CARDS);

		// If the player has at least one card which matches the mask
		// then one such card should be played
		if(
			(m_data.hands[exist_trick_info->player] & exist_trick_info->mask) &&
			!(exist_trick_info->mask & (((unsigned long)1) << in_trick_info->card))
		)
			return gmERR_TRICK_MASK_MISMATCH;

		// Check whether the card played actually exists in the players hand
		if(!(m_data.hands[exist_trick_info->player] & (((unsigned long)1) << in_trick_info->card)))
			return gmERR_TRICK_CARD_NOT_IN_HAND;

		// If the first card to be played in the round
		if(!m_data.tricks[m_data.trick_round].count) {
			m_data.tricks[m_data.trick_round].lead_loc = exist_trick_info->player;
			m_data.tricks[m_data.trick_round].lead_suit = gmGetSuit(in_trick_info->card);
			m_data.tricks[m_data.trick_round].winner = exist_trick_info->player;

			// If the trump has been shown and the first card
			// to be played is a trump then the trick is already trumped
			if(m_data.trump_shown && (gmGetSuit(in_trick_info->card) == m_data.trump_suit))
				m_data.tricks[m_data.trick_round].trumped = true;
		} else {

			// If the card played is a trump
			if(gmGetSuit(in_trick_info->card) == m_data.trump_suit) {
				// and if the trick is already trumped
				if(m_data.tricks[m_data.trick_round].trumped) {
					// check for over trumping
					if(gmGetValue(in_trick_info->card) > gmGetValue(gmWinnerCard)) {
						m_data.tricks[m_data.trick_round].winner = exist_trick_info->player;
					}
				}
				// If the trick is not trumped yet
				// and the trump has been shown,
				else if(m_data.trump_shown) {
					// The trick is being trumped
					m_data.tricks[m_data.trick_round].trumped = true;

					// If the lead suit is trump
					// then check whether we have a new winner
					if(m_data.tricks[m_data.trick_round].lead_suit == m_data.trump_suit) {
						if(gmGetValue(in_trick_info->card) > gmGetValue(gmWinnerCard)) {
							m_data.tricks[m_data.trick_round].winner = exist_trick_info->player;
						}
					}
					// If the lead suit is not trump
					// then we have a new winner
					else {
						m_data.tricks[m_data.trick_round].winner = exist_trick_info->player;
					}
				}
				// If the trick is not trumped yet
				// and if the trump has not been shown
				// but if trump is the lead suit :)
				// check if we have a new trump
				else if(
					(m_data.trump_suit == m_data.tricks[m_data.trick_round].lead_suit) &&
					(gmGetValue(in_trick_info->card) > gmGetValue(gmWinnerCard))
				) {
					m_data.tricks[m_data.trick_round].winner = exist_trick_info->player;
				}
			}
			// If the card being played is not a trump
			else {
				// If the trick is not trumped already,
				// and if the lead suit has been followed,
				// check whether we have a new winner
				if(
					(gmGetSuit(in_trick_info->card) == m_data.tricks[m_data.trick_round].lead_suit) &&
					(gmGetValue(in_trick_info->card) > gmGetValue(gmWinnerCard)) &&
					!m_data.tricks[m_data.trick_round].trumped
				) {
					m_data.tricks[m_data.trick_round].winner = exist_trick_info->player;
				}
			}
		}

		// Common changes in all cases

		// Set the card
		m_data.tricks[m_data.trick_round].cards[exist_trick_info->player] = in_trick_info->card;
		// Remove the card from the hand of the player who played it
		m_data.hands[exist_trick_info->player] &= ~(((unsigned long)1) << in_trick_info->card);
		// Add the card to the list of cards played by the player
		m_data.played_cards[exist_trick_info->player] |= (((unsigned long)1) << in_trick_info->card);
		// Increment the number of cards played in this round
		m_data.tricks[m_data.trick_round].count++;
		// Increment the total points
		m_data.tricks[m_data.trick_round].points += gmUtil::m_points[gmGetValue(in_trick_info->card)];

		m_data.should_play_trump_card = false;
		m_data.should_trump = false;

		// To Remove
		//wxLogDebug(wxString::Format("%s played card %s%s",
		//	gmUtil::m_long_locs[in_trick_info->player].c_str(),
		//	gmUtil::m_suits[gmGetSuit(in_trick_info->card)].c_str(),
		//	gmUtil::m_values[gmGetValue(in_trick_info->card)].c_str()));

		// If all 4 cards have been played, move to the next round
		if(m_data.tricks[m_data.trick_round].count == gmTOTAL_PLAYERS) {
			m_data.trick_round++;
			m_data.tricks[m_data.trick_round].lead_loc = m_data.tricks[m_data.trick_round - 1].winner;
			// To Remove
			//wxLogDebug(wxString::Format("Trick %d completed. Winner - %s.",
			//	m_data.trick_round, gmUtil::m_long_locs[m_data.tricks[m_data.trick_round - 1].winner].c_str() ));

			// Add points to the total of the winning team
			m_data.pts[m_data.tricks[m_data.trick_round - 1].winner % gmTOTAL_TEAMS] += m_data.tricks[m_data.trick_round - 1].points;
		}

		break;
	}

	m_data.input_pending = false;

	return 0;
}

bool gmEngine::GetFeedback() {
	return m_data.feedback;
}
void gmEngine::SetFeedback(bool feedback) {
	m_data.feedback = feedback;
	if(!m_data.feedback)
		m_data.output_pending = false;
}

void gmEngine::GetRules(gmRules *rules) {
	*rules = m_data.rules;
}
void gmEngine::SetRules(gmRules *rules) {
	m_data.rules = *rules;
}
void gmEngine::GetHands(unsigned long *hands) {
	for(int i=0; i<gmTOTAL_PLAYERS; i++) {
		hands[i] = m_data.hands[i];
	}
}
void gmEngine::GetCardsPlayed(unsigned long *cards) {
	for(int i=0; i<gmTOTAL_PLAYERS; i++) {
		cards[i] = m_data.played_cards[i];
	}
}
void gmEngine::GetTrick(int trick_round, gmTrick *trick) {
	wxASSERT((trick_round >= 0) && (trick_round < gmTOTAL_TRICKS));
	*trick = m_data.tricks[trick_round];
}
void gmEngine::GetTrick(gmTrick *trick) {
	GetTrick(m_data.trick_round, trick);
}

int gmEngine::GetTrickRound() {
	return m_data.trick_round;
}

int gmEngine::GetPoints(int team) {
	wxASSERT((team >= 0) && (team < gmTOTAL_TEAMS));
	return m_data.pts[team];
}
void gmEngine::GetPoints(int *pts) {
	wxASSERT(pts);
	for(int i=0; i<gmTOTAL_TEAMS; i++) {
		pts[i] = m_data.pts[i];
	}
}

int gmEngine::GetTrump() {
	return m_data.trump_suit;
}
int gmEngine::GetTrumpCard() {
	return m_data.trump_card;
}
int gmEngine::GetDealer() {
	wxASSERT((m_data.dealer >= 0) && (m_data.dealer < gmTOTAL_PLAYERS));
	return m_data.dealer;
}
void gmEngine::SetDealer(int dealer) {
	wxASSERT((dealer >= 0) && (dealer < gmTOTAL_PLAYERS));
	m_data.dealer = dealer;
}

void gmEngine::ResetTrick(gmTrick *trick) {
	int j;
	for(j = 0; j < gmTOTAL_PLAYERS; j++)
		trick->cards[j] = gmCARD_INVALID;
	trick->count = 0;
	trick->lead_loc = gmPLAYER_INVALID;
	trick->lead_suit = gmSUIT_INVALID;
	trick->points = 0;
	trick->trumped = false;
	trick->winner = gmPLAYER_INVALID;
}

bool gmEngine::GetData(gmEngineData *data) {
	*data = m_data;
	return true;
}

bool gmEngine::SetData(gmEngineData *data, bool check) {
	// TODO : Add error checks and remove wxASSERT
	wxASSERT(!check);
	m_data = *data;
	return true;
}

bool gmEngine::GetMaxBid(int *bid, int *loc) {
	if(bid)
		*bid = m_data.curr_max_bid;
	if(loc)
		*loc = m_data.curr_max_bidder;
	return true;
}
wxString gmEngine::GetLoggable() {
	return PrintRuleEngineData(&m_data);
	/*wxString out;//, temp;
	int i, j;
	out.Append("\n");
	if(m_data.trump_shown)
	{
		out.Append(wxString::Format("Trump - %s(%s)\n",
			gmUtil::m_suits[gmGetSuit(m_data.trump_card)].c_str(),
			gmUtil::m_values[gmGetValue(m_data.trump_card)].c_str()
			));
	}
	for(i = 0; i < m_data.trick_round; i++)
	{
		out.Append(wxString::Format("Trick %d - ", i));

		for(j = 0; j < gmTOTAL_PLAYERS; j++)
		{

		if(m_data.tricks[i].lead_loc == j)
			out.Append("+");
		if(m_data.tricks[i].winner == j)
			out.Append("*");
		out.Append(wxString::Format("%s%s ",
			gmUtil::m_suits[gmGetSuit(m_data.tricks[i].cards[j])].c_str(),
			gmUtil::m_values[gmGetValue(m_data.tricks[i].cards[j])].c_str()
			));
		}
		out.Append("\n");
	}
	out.Append("\n");
	i = m_data.trick_round;
	for(j = 0; j < gmTOTAL_PLAYERS; j++)
	{
		if(m_data.tricks[i].cards[j] == gmCARD_INVALID)
			continue;
		out.Append(wxString::Format("%s - ", gmUtil::m_short_locs[j].c_str()));
		if(m_data.tricks[i].lead_loc == j)
			out.Append("+");
		if(m_data.tricks[i].winner == j)
			out.Append("*");
		out.Append(wxString::Format("%s%s ",
			gmUtil::m_suits[gmGetSuit(m_data.tricks[i].cards[j])].c_str(),
			gmUtil::m_values[gmGetValue(m_data.tricks[i].cards[j])].c_str()
			));
		out.Append("\n");
	}
	out.Append("\n");
	out.Append(gmUtil::PrintHands(m_data.hands));
	return out;*/
}
wxString gmEngine::PrintRuleEngineData(gmEngineData *data) {
	wxString out;//, temp;
	int i, j;

	wxASSERT(data);

	out.Append(wxT("\n"));
	if(data->trump_shown) {
		out.Append(wxString::Format(wxT("Trump - %s(%s)\n"),
									gmUtil::m_suits[gmGetSuit(data->trump_card)].c_str(),
									gmUtil::m_values[gmGetValue(data->trump_card)].c_str()
								   ));
	}
	for(i = 0; i < data->trick_round; i++) {
		out.Append(wxString::Format(wxT("Trick %d - "), i));

		for(j = 0; j < gmTOTAL_PLAYERS; j++) {

			if(data->tricks[i].lead_loc == j)
				out.Append(wxT("+"));
			if(data->tricks[i].winner == j)
				out.Append(wxT("*"));
			out.Append(wxString::Format(wxT("%s%s "),
										gmUtil::m_suits[gmGetSuit(data->tricks[i].cards[j])].c_str(),
										gmUtil::m_values[gmGetValue(data->tricks[i].cards[j])].c_str()
									   ));
		}
		out.Append(wxT("\n"));
	}
	out.Append(wxT("\n"));
	i = data->trick_round;
	for(j = 0; j < gmTOTAL_PLAYERS; j++) {
		if(data->tricks[i].cards[j] == gmCARD_INVALID)
			continue;
		out.Append(wxString::Format(wxT("%s - "), gmUtil::m_short_locs[j].c_str()));
		if(data->tricks[i].lead_loc == j)
			out.Append(wxT("+"));
		if(data->tricks[i].winner == j)
			out.Append(wxT("*"));
		out.Append(wxString::Format(wxT("%s%s "),
									gmUtil::m_suits[gmGetSuit(data->tricks[i].cards[j])].c_str(),
									gmUtil::m_values[gmGetValue(data->tricks[i].cards[j])].c_str()
								   ));
		out.Append(wxT("\n"));
	}
	out.Append(wxT("\n"));
	out.Append(gmUtil::PrintHands(data->hands));
	return out;
}
bool gmEngine::IsTrumpShown() {
	return m_data.trump_shown;
}
int gmEngine::GetTrickNextToPlay() {
	if(m_data.status != gmSTATUS_TRICKS)
		return gmPLAYER_INVALID;
	if(m_data.tricks[m_data.trick_round].count == gmTOTAL_PLAYERS)
		return gmPLAYER_INVALID;

	return gmTrickNext;
}

void gmEngine::SetMinBid(int round, int bid) {
	wxASSERT(round == raBID_ROUND_3);
	wxASSERT((bid == 23) || (bid == 24));
	m_data.rules.min_bid_3 = bid;
}
void gmEngine::SetWaiveRuleFour(bool flag) {
	m_data.rules.waive_rule_4 = flag;
}
void gmEngine::SetSluffJacks(bool flag) {
	m_data.rules.sluff_jacks = flag;
}

//
// Private methods
//

void gmEngine::SetOutput(int output_type) {
	wxASSERT(!m_data.output_pending);
	wxASSERT(!m_data.input_pending);
	m_data.output_type = output_type;
	m_data.output_pending = true;
}
void gmEngine::SetInput(int input_type) {
	wxASSERT(!m_data.output_pending);
	wxASSERT(!m_data.input_pending);
	m_data.input_type = input_type;
	m_data.input_pending = true;
}
unsigned long gmEngine::GenerateMask(unsigned long *rules) {
	unsigned long mask = gmALL_CARDS;
	unsigned long temp = 0;

	// Rule 1 :
	// If the first card to be played in the round
	// and if trump is not shown, the player who set the trump
	// cannot lead a trump.
	if(
		!m_data.tricks[m_data.trick_round].count &&
		!m_data.trump_shown &&
		(gmTrickNext == m_data.curr_max_bidder)) {
		wxASSERT(m_data.trump_suit != gmSUIT_INVALID);
		mask = ~(gmUtil::m_suit_mask[m_data.trump_suit]);
		temp |= gmRULE_1;
	}
	// Rule 4 :
	// If the max bidder asked for trump to be shown
	// he/she must play the very same card
	else if(m_data.should_play_trump_card && (!m_data.rules.waive_rule_4)) {
		wxASSERT(m_data.trump_card != gmCARD_INVALID);
		mask  = ((unsigned long)1) << m_data.trump_card;
		temp |= gmRULE_4;
	}
	// Rule 2 :
	// If trump was asked to be shown, then trump must be played
	else if(m_data.should_trump) {
		wxASSERT(m_data.trump_suit != gmSUIT_INVALID);
		mask = gmUtil::m_suit_mask[m_data.trump_suit];
		temp |= gmRULE_2;
	}
	// Rule 3 :
	// Should follow suit
	else if(m_data.tricks[m_data.trick_round].count) {
		wxASSERT(m_data.tricks[m_data.trick_round].lead_suit != gmSUIT_INVALID);
		mask = gmUtil::m_suit_mask[m_data.tricks[m_data.trick_round].lead_suit];
		temp |= gmRULE_3;
	}
	wxASSERT(mask);

	// Set the rules which were considered
	if(rules)
		*rules = temp;
	return mask;
}
bool gmEngine::SetDealEndOutput() {
	// Check whether there is a winner?
	if(m_data.pts[m_data.curr_max_bidder % 2] >= m_data.curr_max_bid) {
		m_data.out_deal_end_info.winner = m_data.curr_max_bidder % 2;
	}
	// TODO : Remove hard coding of 28
	else if(m_data.pts[(m_data.curr_max_bidder + 1) % 2] > (28 - m_data.curr_max_bid)) {
		m_data.out_deal_end_info.winner = (m_data.curr_max_bidder + 1) % 2;
	} else
		return false;
	if(m_data.feedback)
		SetOutput(gmOUTPUT_DEAL_END);
	return true;
}
