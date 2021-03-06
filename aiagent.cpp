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

#include "aiagent.h"
#include "aiEval.h"
#include "gmTrick.h"

#define raAI_ORDERMOVES

const int aiAgent::s_depths[] = {2, 3, 4, 6, 7, 8, 8, 8};

aiAgent::aiAgent() {
	// TODO : Remove hardcoding
	m_loc = 0;
	Reset();
}

aiAgent::~aiAgent() {
}

//
// Public method/s
//
void aiAgent::SetLocation(int loc) {
	m_loc = loc;
}

int aiAgent::GetLocation() {
	return m_loc;
}

bool aiAgent::GetBid(unsigned long cards, int *bid, int *trump, int min, bool force_bid) {
	int data[4][29];
	for(int i=0;i<4;i++){
		for(int j=0;j<29;j++){
			data[i][j]= 0;
		}
	}

#ifdef raAI_LOG_GETBID
	wxLogDebug(gmUtil::PrintLong(cards));
#endif

	int initial = (int)gmUtil::CountBitsSet(cards);
	wxASSERT(initial <= 8);
#ifdef raAI_LOG_GETBID
	wxLogDebug(wxString::Format("initial is %d", initial));
#endif
	int *undealt = new int[32 - initial];

	//
	//Dealing the rest of the cards
	//

	for(int sample = 0; sample < aiBID_SAMPLE; sample++) {
		// Get the rest of the cards into undealt
		int k = 0;
		for(int i = 0; i < 32; i++)
			if(!(cards & (((unsigned long)1) << i)))
				undealt[k++] = i;

		//Shuffle the undealt
		gmUtil::ShuffleArray(undealt, 32 - initial);

		//Initialize the hands
		unsigned long hands[4];
		for (int i=0; i<4; i++){
			hands [i]=0;
		}

		// Simplifying the problem, assume the current player is South
		// Add the cards already dealt to South
		hands[0] = cards;


		// Deal the undealt cards
		k = 0;
		for(int i = /*8 - */initial; i < 32; i++)
			hands[i / 8] |= ((unsigned long)1) << undealt[k++];

#ifdef raAI_LOG_GETBID
		wxLogDebug("#############################");
		wxLogDebug(gmUtil::PrintHands(hands));
#endif


		// Calculate the estimated points
		// considering each suit as trump

		for(int trump_count = 0; trump_count < 4; trump_count++) {
			// Estimation done only if there is atleast one card
			// belonging to the suit, in the initial hand
			if(cards & gmUtil::m_suit_mask[trump_count]) {
				int temp_trump = trump_count;
				int eval[2] = {0, 0};
				EstimatePoints(hands, temp_trump, 0, eval);

				// Distribute half of the unallocated points equally
				// This is pure guess work
				eval[0] += (28 - eval[0] - eval[1]) / 4;

#ifdef raAI_LOG_GETBID
				if((eval[0] > 28) || eval[0] < 0) {
					wxLogDebug(wxString::Format("RRRooor %d %d", eval[0], eval[1]));
					wxLogDebug(wxString::Format("%08X %08X %08X %08X", hands[0], hands[1], hands[2], hands[3]));
					wxLogDebug(gmUtil::m_suits[trump_count].c_str());
				}
				wxLogDebug(wxString::Format("%s : %d - %d", gmUtil::m_suits[trump_count].c_str(), eval[0], eval[1]));
#endif
				data[trump_count][eval[0]]++;
			}
		}
#ifdef raAI_LOG_GETBID
		//wxLogDebug("#############################");
#endif
	}
#ifdef raAI_LOG_GETBID
	wxLogDebug("#############################");
	for(i = 0; i < 4; i++) {
		wxLogDebug(_("Trump - ") + gmUtil::m_suits[i]);
		for(k = 0; k < 29; k++) {
			wxLogDebug(wxString::Format("Bid - %d : %d", k, data[i][k]));
		}
	}
	wxLogDebug("#############################");
#endif


	// Analyzing cumulative success percentages and
	// figuring out which is the best bid
	*bid = 0;
	*trump = -1;
	for(int i = 0; i < 4; i++) {
		int sample = 0;
		for(int k = 28; k >= 0; k--) {
			sample += data[i][k];
			// TODO : 70 might be an aggressive value fix it accordingly
			if((((double)(sample) / aiBID_SAMPLE) >= 0.67) && (k > *bid)) {
				*bid = k;
				*trump = i;
			}
		}
	}

#ifdef raAI_LOG_GETBID
	wxLogDebug(wxString::Format("Optimal bid is %d %d", *bid, *trump));
#endif

	delete undealt;

	// If the suggested bid is less than
	// the minimum bid suggested, return pass
	// or minimum bid appropriately
	if(*bid < min) {
		if(force_bid)
			*bid = min;
		else {
			*bid = 0;
			*trump = -1;
		}
	}

	return true;
}

bool aiAgent::GetBid(int *bid, int *trump, int min, bool force_bid) {
	int ret_trump;
	int ret_bid;
	unsigned long hands[4];
	unsigned long cards;
	int max_bidder;
	int trump_card;

	wxASSERT(bid);
	wxASSERT(trump);
	// TODO : Correct this check
	// Check if the current Rule Engine state is auction
	if(m_engine.GetStatus() == gmSTATUS_NOT_STARTED) {
		return false;
	}

	m_engine.GetHands(hands);
#ifdef raAI_LOG_GETBID
	wxLogDebug(wxString::Format("Estimated bid for %d", m_loc));
	wxLogDebug(gmUtil::PrintLong(hands[m_loc]));
#endif

	cards = hands[m_loc];
	wxASSERT(cards);

	max_bidder = gmPLAYER_INVALID;
	m_engine.GetMaxBid(NULL, &max_bidder);
	wxASSERT((max_bidder >= gmPLAYER_INVALID) && (max_bidder < gmTOTAL_PLAYERS));

	if(max_bidder == m_loc) {
		trump_card = m_engine.GetTrumpCard();
		wxASSERT((trump_card >= 0) && (trump_card < gmTOTAL_CARDS));
		cards |= (((unsigned long)1) << trump_card);
	}

	wxASSERT(gmUtil::CountBitsSet(cards) <= 8);

	if(!GetBid(cards, &ret_bid, &ret_trump, min, force_bid)) {
		wxLogDebug(wxString::Format(wxT("GetBid failed. File - %s Line - %d"), wxT(__FILE__), __LINE__));
		return false;
	}
	wxASSERT((ret_bid == gmBID_ALL) || (ret_bid == gmBID_PASS) || (ret_bid >= min));
	*bid = ret_bid;
	*trump = ret_trump;

	return true;
}

bool aiAgent::SetRuleEngineData(gmEngineData *data) {
	m_engine.SetData(data, false);
	return true;
}

int aiAgent::GetTrump() {
	int bid;
	int trump;
	unsigned long hands[gmTOTAL_PLAYERS];

	m_engine.GetHands(hands);

	GetBid(&bid, &trump, 14, true);

	return GetTrump(hands[m_loc], trump);
}

// Function returns the card to be played(index)
// -1, for show trump
// -2 or other negative values in case of error
int aiAgent::GetTrump(unsigned long hand, int suit) {
	int i;
	unsigned long trump_cards;
	int ret_val = gmCARD_INVALID;

	wxASSERT(hand);
	wxASSERT((suit > gmSUIT_INVALID) && (suit <= gmTOTAL_SUITS));

	trump_cards = (hand & gmUtil::m_suit_mask[suit]) >> gmUtil::m_suit_rs[suit];

	// If there are two cards with points
	// select smallest as the trump
	if((gmUtil::CountBitsSet(trump_cards & 0x0000000F0)) >= 2) {
		for(i = 4; i < 8; i++)
			if(trump_cards & (((unsigned long)1) << i)) {
				ret_val = (suit * 8) + i;
				break;
			}
	}
	// Else, if you have at least on card
	// with no points select the highest as the trump
	else if(gmUtil::CountBitsSet(trump_cards & 0x0000000F) > 0) {
		ret_val = (suit * 8) + gmUtil::HighestBitSet(trump_cards & 0x0000000F);
	}
	// Else, select the highest card as trump
	else {
		ret_val = (suit * 8) + gmUtil::HighestBitSet(trump_cards);
	}

	// Make sure that the trump card selected exists in the hand
	wxASSERT(hand & (((unsigned long)1) << ret_val));
	return ret_val;
}

int aiAgent::GetPlay(unsigned long mask) {
	gmEngineData data, work_data, bkp_data;
	gmEngine rule_engine;
	unsigned long **deal_hands = NULL;
	int i, j, k;
	aiEval evals[gmTOTAL_VALUES];
	aiEval eval_trump, avg_eval_trump;

	aiMove moves[gmTOTAL_VALUES];
	aiMove moves_trump[gmTOTAL_VALUES];
	int move_count, move_trump_count;
	int depth = -1;
	int eval;

	double best_eval;
	double best_eval_trump;
	double temp_eval;
	int best_play;

#ifdef raAI_LOG_GET_PLAY
	wxLogDebug("**********Entering GetPlay()*****************");
	wxLogDebug(wxString::Format("Location - %s", gmUtil::m_long_locs[m_loc].c_str()));
#endif

	if(!m_engine.GetData(&data)) {
		wxLogError(wxString::Format(wxT("GetData() failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return -2;
	}
#ifdef raAI_LOG_GET_PLAY
	wxLogDebug("Rule engine data :");
	wxLogDebug(m_engine.GetLoggable());
	wxLogDebug("Trump candidates :");
	for(i = 0; i < gmTOTAL_SUITS; i++) {
		if(m_trump_cards & (((unsigned long)1) << i))
			wxLogDebug(gmUtil::m_suits[i].c_str());
	}
#endif

	wxASSERT((m_trump_cards >= 0) && (m_trump_cards <= 15));

	// Create the array to hold the random deals
	deal_hands = new unsigned long *[30];
	if(!deal_hands) {
		wxLogError(wxString::Format(wxT("Memory allocation failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return -2;
	}
	for (int i=0; i<30; i++){
		deal_hands[i]=NULL;
	}
	for(i = 0; i < 30; i++) {
		deal_hands[i] = new unsigned long[gmTOTAL_PLAYERS];
		if(!deal_hands[i]) {
			wxLogError(wxString::Format(wxT("Memory allocation failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return -2;
		}
		for (int j=0; j<gmTOTAL_PLAYERS; j++){
			deal_hands[i][j]=0;
		}
	}

	//
	// Generate deals and moves, play each and find the best move
	//
	for(int i=0; i<gmTOTAL_VALUES; i++){
		evals[i].eval=0;
		evals[i].count=0;
		evals[i].valid=false;
	}

	work_data = data;
	bkp_data = work_data;

	rule_engine.SetData(&work_data, false);

	depth = s_depths[m_engine.GetTrickRound()];
#ifdef raAI_LOG_GET_PLAY
	wxLogDebug(wxString::Format("Depth of search - %d", depth));
#endif

	avg_eval_trump.count = 0;
	avg_eval_trump.eval = 0;
	avg_eval_trump.valid = false;


	// If trump is shown or if self is the max bidder
	// then trump is known.
	if(data.trump_shown || (m_loc == data.curr_max_bidder)) {
#ifdef raAI_LOG_GET_PLAY
		if(data.trump_shown)
			wxLogDebug("Trump known as trump is shown");
		else
			wxLogDebug("Trump known as m_loc is the max bidder");
#endif
		// Generate possible moves which do not ask for trump
		if(!GenerateMoves(&rule_engine, moves, &move_count, aiGENMV_NOTRUMP)) {
			wxLogError(wxString::Format(wxT("GenerateMoves() failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return -2;
		}
		wxASSERT(move_count > 0);

#ifdef raAI_LOG_GET_PLAY
		wxLogDebug("Moves generated (without asking for trump) :");
		wxLogDebug(PrintMoves(moves, move_count));
#endif

		// Generate moves which ask for trump
		if(!GenerateMoves(&rule_engine, moves_trump, &move_trump_count, aiGENMV_TRUMP)) {
			wxLogError(wxString::Format(wxT("GenerateMoves() failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return -2;
		}
		wxASSERT(move_trump_count >= 0);

#ifdef raAI_LOG_GET_PLAY
		wxLogDebug("Moves generated (asking for trump) :");
		wxLogDebug(PrintMoves(moves_trump, move_trump_count));
#endif

		// Generate random deals
		if(!GenerateDeals(&data, deal_hands, 30)) {
			wxLogError(wxString::Format(wxT("GenerateDeals() failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return -2;
		}
		// For each random deal
		for(i = 0; i < 30; i++) {
			for(int h=0; h<gmTOTAL_PLAYERS; h++) {
				work_data.hands[h] = deal_hands[i][h];
			}

#ifdef raAI_LOG_GET_PLAY
			wxLogDebug(wxString::Format("Random deal no : %d", i));
#endif

			for(j = 0; j < move_count; j++) {
				rule_engine.SetData(&work_data, false);

#ifdef raAI_LOG_GET_PLAY
				wxLogDebug("----------------------------------------------");
				wxLogDebug(wxString::Format("Random deal no : %d", i));
				wxLogDebug(wxString::Format("Attempting move no : %d", j));
				wxLogDebug(PrintMoves(&moves[j], 1));
				wxLogDebug("Rule engine data dump :");
				wxLogDebug(rule_engine.GetLoggable());
#endif
				if(!MakeMoveAndEval(&rule_engine, &moves[j], depth, &eval)) {
					wxLogError(wxString::Format(wxT("MakeMoveAndEval() failed. %s:%d"),
												wxT(__FILE__), __LINE__));
					return -2;
				}
				evals[j].eval += eval;
				evals[j].count++;
				evals[j].valid = true;
#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("Eval - %d", eval));
				wxLogDebug(wxString::Format("evals[%d].eval = %d", j, evals[j].eval));
				wxLogDebug(wxString::Format("evals[%d].count = %d", j, evals[j].count));
#endif
			}

			eval_trump.count = 0;
			eval_trump.eval = aiNEG_INFTY;
			eval_trump.valid = false;

			for(j = 0; j < move_trump_count; j++) {
				rule_engine.SetData(&work_data, false);

#ifdef raAI_LOG_GET_PLAY
				wxLogDebug("----------------------------------------------");
				wxLogDebug(wxString::Format("Random deal no : %d", i));
				wxLogDebug(wxString::Format("Attempting move no : %d", j));
				wxLogDebug(PrintMoves(&moves_trump[j], 1));
				wxLogDebug("Rule engine data dump :");
				wxLogDebug(rule_engine.GetLoggable());
#endif
				if(!MakeMoveAndEval(&rule_engine, &moves_trump[j], depth, &eval)) {
					wxLogError(wxString::Format(wxT("MakeMoveAndEval() failed. %s:%d"),
												wxT(__FILE__), __LINE__));
					return -2;
				}
				if(eval > eval_trump.eval) {
					eval_trump.eval = eval;
					eval_trump.valid = true;
				}
#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("Eval - %d", eval));
				wxLogDebug(wxString::Format("eval_trump.eval = %d", eval_trump.eval));
#endif
			}

			if(j > 0) {
				wxASSERT(eval_trump.valid);
				avg_eval_trump.count++;
				avg_eval_trump.eval += eval_trump.eval;
#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("avg_eval_trump.count - %d",avg_eval_trump.count));
				wxLogDebug(wxString::Format("avg_eval_trump.eval - %d",avg_eval_trump.eval));
#endif
			}

			// Update the statusbar
			gmUtil::SetStatusText(wxString::Format(
									  wxT("%s is thinking - %d%%"), gmUtil::m_long_locs[m_loc].c_str(),
									  (i * 100) / 30 ), raSBARPOS_GEN);

		}

		gmUtil::SetStatusText(wxT(""), raSBARPOS_GEN);

		best_eval = (double)aiNEG_INFTY;
		best_eval_trump = (double)aiNEG_INFTY;
		best_play = gmCARD_INVALID;

#ifdef raAI_LOG_GET_PLAY
		wxLogDebug("Average evals for each move :");
#endif

		// Find out the best play without asking for the trump
		for(j = 0; j < move_count; j++) {
			if(evals[j].valid) {
				temp_eval = (double)evals[j].eval / (double)evals[j].count;

#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("%s - %5.2f", PrintMoves(&moves[j], 1).c_str(), temp_eval));
#endif

				if(temp_eval > best_eval) {
					best_eval = temp_eval;
					best_play = moves[j].card;
				}
			}
		}
		wxASSERT(best_play != gmCARD_INVALID);

#ifdef raAI_LOG_GET_PLAY
		wxLogDebug(wxString::Format("Best play (only considering cases where trump is not asked) - %s%s",
									gmUtil::m_suits[gmGetSuit(best_play)],
									gmUtil::m_values[gmGetValue(best_play)]));
#endif

		// Compare the options -
		// Ask for trump or play without asking for trump
		if(avg_eval_trump.count > 0) {
			best_eval_trump = (double)avg_eval_trump.eval / (double)avg_eval_trump.count;
#ifdef raAI_LOG_GET_PLAY
			wxLogDebug(wxString::Format("Best_eval_trump - %5.2f", best_eval_trump));
#endif
			//wxASSERT(best_play_trump != gmCARD_INVALID);
			if(best_eval_trump > best_eval) {
#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("Best_eval - %5.2f", best_eval));
				wxLogDebug("Best move is to ask for trump");
#endif
				best_play = -1;//best_play_trump;
			}
		}

	}
	// If trump is not shown and if self is not the max bidder
	// then trump is not known. Consider each suit as
	// a possible trump
	else {
#ifdef raAI_LOG_GET_PLAY
		wxLogDebug("Trump is not known");
#endif
		// Generate possible moves which do not ask for trump
		if(!GenerateMoves(&rule_engine, moves, &move_count, aiGENMV_NOTRUMP)) {
			wxLogError(wxString::Format(wxT("GenerateMoves() failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return -2;
		}
		wxASSERT(move_count > 0);

		wxASSERT(m_trump_cards);

#ifdef raAI_LOG_GET_PLAY
		wxLogDebug("Moves generated (without asking for trump) :");
		wxLogDebug(PrintMoves(moves, move_count));
#endif
		for(k = 0; k < gmTOTAL_SUITS; k++) {
			// If the trump is not possible ignore
			if(!(m_trump_cards & (((unsigned long)1) << k)))
				continue;

			work_data.trump_suit = k;
			rule_engine.SetData(&work_data, false);

			// Generate moves which ask for trump
			if(!GenerateMoves(&rule_engine, moves_trump, &move_trump_count, aiGENMV_TRUMP)) {
				wxLogError(wxString::Format(wxT("GenerateMoves() failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				return -2;
			}
			wxASSERT(move_trump_count >= 0);

#ifdef raAI_LOG_GET_PLAY
			wxLogDebug(wxString::Format("Trump - %s", gmUtil::m_suits[k].c_str()));
			wxLogDebug("Moves generated (asking for trump) :");
			wxLogDebug(PrintMoves(moves_trump, move_trump_count));
#endif

			// Generate random deals
			if(!GenerateDeals(&work_data, deal_hands, 30, k)) {
				wxLogError(wxString::Format(wxT("GenerateDeals() failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				return -2;
			}
			// For each random deal
			for(i = 0; i < 30; i++) {
				for(int h=0; h<gmTOTAL_PLAYERS; h++) {
					work_data.hands[h] = deal_hands[i][h];
				}

#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("Random deal no : %d", i));
#endif

				// Set the trump card
				work_data.trump_card = GetTrump(work_data.hands[work_data.curr_max_bidder], work_data.trump_suit);

				wxASSERT((work_data.trump_card > gmCARD_INVALID) && (work_data.trump_card < gmTOTAL_CARDS));
				wxASSERT(work_data.hands[work_data.curr_max_bidder] & (((unsigned long)1) << work_data.trump_card));

				// Remove the trump card from the max bidder's hand
				work_data.hands[work_data.curr_max_bidder] &= ~(((unsigned long)1) << work_data.trump_card);

				for(j = 0; j < move_count; j++) {
					rule_engine.SetData(&work_data, false);
#ifdef raAI_LOG_GET_PLAY
					wxLogDebug("----------------------------------------------");
					wxLogDebug(wxString::Format("Random deal no : %d", i));
					wxLogDebug(wxString::Format("Trump : %s", gmUtil::m_suits[k].c_str()));
					wxLogDebug(wxString::Format("Attempting move no : %d", j));
					wxLogDebug(PrintMoves(&moves[j], 1));
					wxLogDebug("Rule engine data dump :");
					wxLogDebug(rule_engine.GetLoggable());
#endif
					if(!MakeMoveAndEval(&rule_engine, &moves[j], depth, &eval)) {
						wxLogError(wxString::Format(wxT("MakeMoveAndEval() failed. %s:%d"),
													wxT(__FILE__), __LINE__));
						return -2;
					}
					evals[j].eval += eval;
					evals[j].count++;
					evals[j].valid = true;
#ifdef raAI_LOG_GET_PLAY
					wxLogDebug(wxString::Format("Eval - %d", eval));
					wxLogDebug(wxString::Format("evals[%d].eval = %d", j, evals[j].eval));
					wxLogDebug(wxString::Format("evals[%d].count = %d", j, evals[j].count));
#endif
				}

				rule_engine.SetData(&work_data, false);
				// Generate moves which ask for trump
				if(!GenerateMoves(&rule_engine, moves_trump, &move_trump_count, aiGENMV_TRUMP)) {
					wxLogError(wxString::Format(wxT("GenerateMoves() failed. %s:%d"),
												wxT(__FILE__), __LINE__));
					return -2;
				}
				wxASSERT(move_trump_count >= 0);

				eval_trump.count = 0;
				eval_trump.eval = aiNEG_INFTY;
				eval_trump.valid = false;

				for(j = 0; j < move_trump_count; j++) {
					rule_engine.SetData(&work_data, false);
#ifdef raAI_LOG_GET_PLAY
					wxLogDebug("----------------------------------------------");
					wxLogDebug(wxString::Format("Random deal no : %d", i));
					wxLogDebug(wxString::Format("Trump : %s", gmUtil::m_suits[k].c_str()));
					wxLogDebug(wxString::Format("Attempting move no : %d", j));
					wxLogDebug(PrintMoves(&moves_trump[j], 1));
					wxLogDebug("Rule engine data dump :");
					wxLogDebug(rule_engine.GetLoggable());
#endif
					if(!MakeMoveAndEval(&rule_engine, &moves_trump[j], depth, &eval)) {
						wxLogError(wxString::Format(wxT("MakeMoveAndEval() failed. %s:%d"),
													wxT(__FILE__), __LINE__));
						return -2;
					}
					if(eval > eval_trump.eval) {
						eval_trump.eval = eval;
						eval_trump.valid = true;
					}
#ifdef raAI_LOG_GET_PLAY
					wxLogDebug(wxString::Format("Eval - %d", eval));
					wxLogDebug(wxString::Format("eval_trump.eval = %d", eval_trump.eval));
#endif
				}
				if(j > 0) {
					wxASSERT(eval_trump.valid);
					avg_eval_trump.count++;
					avg_eval_trump.eval += eval_trump.eval;
#ifdef raAI_LOG_GET_PLAY
					wxLogDebug(wxString::Format("avg_eval_trump.count - %d",avg_eval_trump.count));
					wxLogDebug(wxString::Format("avg_eval_trump.eval - %d",avg_eval_trump.eval));
#endif
				}

				// Update the statusbar
				gmUtil::SetStatusText(wxString::Format(
										  wxT("%s is thinking - %d%%"), gmUtil::m_long_locs[m_loc].c_str(),
										  (((k * 30) + i) * 100) / (gmTOTAL_SUITS * 30) ), raSBARPOS_GEN);

			}
		}

		gmUtil::SetStatusText(wxT(""), raSBARPOS_GEN);

		best_eval = (double)aiNEG_INFTY;
		best_eval_trump = (double)aiNEG_INFTY;
		best_play = gmCARD_INVALID;
#ifdef raAI_LOG_GET_PLAY
		wxLogDebug("Average evals for each move :");
#endif

		// Find out the best play without asking for the trump
		for(j = 0; j < move_count; j++) {
			if(evals[j].valid) {
				temp_eval = (double)evals[j].eval / (double)evals[j].count;
#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("%s - %5.2f", PrintMoves(&moves[j], 1).c_str(), temp_eval));
#endif
				if(temp_eval > best_eval) {
					best_eval = temp_eval;
					best_play = moves[j].card;
				}
			}
		}
		wxASSERT(best_play != gmCARD_INVALID);
#ifdef raAI_LOG_GET_PLAY
		wxLogDebug(wxString::Format("Best play (only considering cases where trump is not asked) - %s%s",
									gmUtil::m_suits[gmGetSuit(best_play)],
									gmUtil::m_values[gmGetValue(best_play)]));
#endif

		// Compare the options -
		// Ask for trump or play without asking for trump
		if(avg_eval_trump.count > 0) {
			// Find out the best play after asking for the trump
			best_eval_trump = (double)avg_eval_trump.eval / (double)avg_eval_trump.count;
#ifdef raAI_LOG_GET_PLAY
			wxLogDebug(wxString::Format("Best_eval_trump - %5.2f", best_eval_trump));
#endif
			if(best_eval_trump > best_eval) {
#ifdef raAI_LOG_GET_PLAY
				wxLogDebug(wxString::Format("Best_eval - %5.2f", best_eval));
				wxLogDebug("Best move is to ask for trump");
#endif
				best_play = -1;//best_play_trump;
			}
		}

	}

	// Free the memory allocated to hold the random deals
	for(i = 0; i < 30; i++) {
		delete[] deal_hands[i];
		deal_hands[i] = NULL;
	}
	delete[] deal_hands;
	deal_hands = NULL;

	wxASSERT((best_play == -1) || ((best_play > gmCARD_INVALID) && (best_play < gmTOTAL_CARDS)));

#ifdef raAI_LOG_GET_PLAY
	wxLogDebug("**********Exiting GetPlay()*****************");
#endif

	return best_play;
}

// Given the game engine data, generate the suit length problem.
// This suit length problem is later on solved to create random deals staisfying
// the existing constraints for Monte Carlo
bool aiAgent::GenerateSLProblem(gmEngineData *data, slProblem *problem, slPlayed played, int trump, bool *add_trump) {
	unsigned long cards_played = 0;
	int i, j;
	int sum_hands = 0, sum_suits = 0;

#ifdef aiLOG_GENERATESLPROBLEM
	wxLogDebug(wxT("Inside GenerateSLProblem"));
	wxLogDebug(wxString::Format(wxT("m_loc - %s"), gmUtil::m_short_locs[m_loc].c_str()));
#endif

	wxASSERT(data);
	wxASSERT(problem);
	wxASSERT(played);

	*add_trump = false;

	// TODO : Provide an appropriate comment
	if((!data->trump_shown) && (data->curr_max_bidder != m_loc)) {
		wxASSERT(trump != gmSUIT_INVALID);
	}

	// TODO : Provide an appropriate comment
	if((data->trump_shown) || (data->curr_max_bidder == m_loc)) {
		trump = data->trump_suit;
	}

	// TODO : Implement the case where the opponents of the max bidder
	// should have at least one trump

	// Initialize the problem. This will set all slots vacant and would default hand and suit total lengths

	aiSuitLengthSolver::InitializeProblem(problem);
	aiSuitLengthSolver::InitializePlayed(played);

	// Set the data for played

	for(i = 0; i < gmTOTAL_PLAYERS; i++) {
		for(j = 0; j < gmTOTAL_SUITS; j++) {
			played[i][j] = gmUtil::CountBitsSet(data->played_cards[i] & gmUtil::m_suit_mask[j]);
		}
	}

	// Set the hand lengths

	for(i = 0; i < gmTOTAL_PLAYERS; i++) {
		problem->hand_total_length[i] = 8 - (gmUtil::CountBitsSet(data->played_cards[i]));
		sum_hands += problem->hand_total_length[i];
		cards_played |= data->played_cards[i];
	}

	// Set suit lengths

	for(i = 0; i < gmTOTAL_SUITS; i++) {
		problem->suit_total_length[i] = 8 - gmUtil::CountBitsSet(cards_played & gmUtil::m_suit_mask[i]);
		sum_suits += problem->suit_total_length[i];
	}

	// This is applicable only if self is not the max bidder.
	// If the trump is not shown, max bidder should have at least one trump. This is fixed.
	// Hence remove this from the total suit length for the max bidder and the trump suit.
	// The suit length should be incremented manually for all solutions for the slot [max bidder][trump].

	if((!data->trump_shown) && (m_loc != data->curr_max_bidder)) {
		--(problem->hand_total_length[data->curr_max_bidder]);
		--(problem->suit_total_length[trump]);
		++(played[data->curr_max_bidder][trump]);
		*add_trump = true;

	}

	// This is applicable only if self is not the player who bid the contract (curr_max_bidder).
	// If the trump is shown, but if the max bidder is yet to play the
	// card that was set as the trump then, max bidder has at least one trump. This is fixed.
	// Hence remove this from the total suit length for the max bidder and the trump suit.
	// The suit length should be incremented manually for all solutions for the slot [max bidder][trump].
	if((data->trump_shown) && (m_loc != data->curr_max_bidder)) {
		if(!(data->played_cards[data->curr_max_bidder] & (((unsigned long)1) << data->trump_card))) {
			--(problem->hand_total_length[data->curr_max_bidder]);
			--(problem->suit_total_length[trump]);
			++(played[data->curr_max_bidder][trump]);
			*add_trump = true;
		}
	}

	// Set the cases where the suit length is null
	for(i = 0; i < gmTOTAL_PLAYERS; i++) {
		for(j = 0; j < gmTOTAL_SUITS; j++) {
			//problem->cells[i][j].max =
			//std::min(problem->hand_total_length[i], problem->suit_total_length[j]);
			if(m_nulls[i] & (((unsigned long)1) << j)) {
				//problem->cells[i][j].min = 0;
				//problem->cells[i][j].max = 0;
				problem->suit_length[i][j] = 0;
			}
		}
	}

	// Set the cells for self
	for(i = 0; i < gmTOTAL_SUITS; i++) {
		problem->suit_length[m_loc][i] = gmUtil::CountBitsSet(data->hands[m_loc] & gmUtil::m_suit_mask[i]);
	}
	// If self is the max bidder and if trump is not shown,
	// add one to the suit length to accommodate the card kept as trump
	if((!data->trump_shown) && (m_loc == data->curr_max_bidder)) {
		++(problem->suit_length[m_loc][trump]);
	}

	wxASSERT(sum_suits == sum_hands);

#ifdef aiLOG_GENERATESLPROBLEM
	wxString out;
	for(i = 0; i < gmTOTAL_PLAYERS; i++) {
		out.Empty();
		out.Append(wxString::Format(wxT("%s - "), gmUtil::m_short_locs[i].c_str()));
		for(j = 0; j < gmTOTAL_SUITS; j++) {
			if(problem->suit_length[i][j] == slVACANT) {
				out.Append(wxT("x "));
			} else {
				out.Append(wxString::Format(wxT("%d "), problem->suit_length[i][j]));
			}

		}
		wxLogDebug(out);
	}
	wxLogDebug(wxT("Exiting GenerateSLProblem"));
#endif

	return true;
}

bool aiAgent::GenerateDeals(gmEngineData *data, unsigned long **deals, int count, int trump) {
	slProblem problem;
	slPlayed played;
	slSolution solution;
	aiSuitLengthSolver solver;
	int to_deal[gmTOTAL_SUITS][gmTOTAL_VALUES];
	int to_deal_count[gmTOTAL_SUITS];
	unsigned long cards_played = 0;
	unsigned long temp;
	unsigned long known_alloc[gmTOTAL_PLAYERS];
	bool add_trump = false;


	wxASSERT(data);

	//
	// Allocate the known cards straightaway
	//
	for(int i=0; i<gmTOTAL_PLAYERS;i++){
		known_alloc[i]=0;
	}

	// Cards beloning to self are known. Allocate those first
	known_alloc[m_loc] = data->hands[m_loc];

	// If self is the max bidder and if the trump is not shown
	// Add the card that is set as the trump to hand
	if((!data->trump_shown) && (m_loc == data->curr_max_bidder))
		known_alloc[m_loc] |= (((unsigned long)1) << data->trump_card);

	// If trump is shown and the card that was set as trump is not
	// played yet, add the same to the max bidders hand
	if(data->trump_shown && (!(data->played_cards[data->curr_max_bidder] & (((unsigned long)1) << data->trump_card))))
		known_alloc[data->curr_max_bidder] |= (((unsigned long)1) << data->trump_card);

	// Consider the allocated cards as played
	for(int i = 0; i < gmTOTAL_PLAYERS; i++) {
		cards_played |= known_alloc[i];
	}

	// Get the list of cards to be dealt for each suit
	for(int i = 0; i < gmTOTAL_SUITS; i++) {
		cards_played |= data->played_cards[i];
		to_deal_count[i] = 0;
	}

	// For each suit, create an array of the cards to be dealt
	for(int i = 0; i < gmTOTAL_SUITS; i++) {
		temp = (~cards_played) & gmUtil::m_suit_mask[i];
		int j = 0;
		while(temp) {
			to_deal[i][j] = gmUtil::HighestBitSet(temp);
			temp &= ~(((unsigned long)1) << to_deal[i][j]);
			j++;
		}
		to_deal_count[i] = j;
	}

	// Generate the problem

	// No need to initialize "problem" and "played" as GenerateSLProblem will do this.
	if(!GenerateSLProblem(data, &problem, played, trump, &add_trump)) {
		wxLogError(wxString::Format(wxT("GetData() failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return false;
	}

	//
	// Generate random deals
	//

	// Set the problem
	solver.SetProblem(&problem, played);
	for(int c = 0; c < count; c++) {
		// Get a random solution
		for(int i=0;i<slTOTAL_HANDS;i++){
			for(int j=0;j<slTOTAL_SUITS;j++){
				solution[i][j]=0;
			}
		}
		solver.GenerateRandomSolution(solution);

		// If the trump card needs to be added to the max bidders hand, do that
		if(add_trump) {
			++(solution[data->curr_max_bidder][data->trump_suit]);
		}

#ifdef aiLOG_GENERATEDEALS

		wxLogDebug(wxT("Final corrected solution"));
		if(add_trump) {
			wxLogDebug(wxT("add_trump is true"));
		} else {
			wxLogDebug(wxT("add_trump is false"));
		}
		wxLogDebug(wxString::Format(wxT("1 added to slot %d, %d"), data->curr_max_bidder, trump));
		wxLogDebug(aiSuitLengthSolver::PrintMatrix(solution));

#endif

		for(int i=0; i<gmTOTAL_PLAYERS; i++) {
			deals[c][i] = known_alloc[i];
		}

		// For each array shuffle the cards to be dealt
		for(int j = 0; j < gmTOTAL_SUITS; j++) {
			gmUtil::ShuffleArray(to_deal[j], to_deal_count[j]);
		}

		// For each player deal the undealt cards
		for(int k = 0; k < gmTOTAL_SUITS; k++) {
			int l = 0;
			for(int j = 0; j < gmTOTAL_PLAYERS; j++) {
				wxASSERT((int)gmUtil::CountBitsSet(deals[c][j] & gmUtil::m_suit_mask[k]) <= solution[j][k]);
				while((int)gmUtil::CountBitsSet(deals[c][j] & gmUtil::m_suit_mask[k]) < solution[j][k]) {
					wxASSERT(l < to_deal_count[k]);
					deals[c][j] |= (((unsigned long)1) << to_deal[k][l]);
					l++;
				}
			}
		}
	}


	return true;
}
wxString aiAgent::PrintMoves(aiMove *moves, int move_count) {
	wxString out;
	int i;
	wxASSERT(move_count >= 0);
	out.Append(wxString::Format(wxT("%d moves - "), move_count));
	for(i = 0; i < move_count; i++) {
		if(moves[i].ask_trump) {
			out.Append(wxString::Format(wxT("?%s%s(%d),"),
										gmUtil::m_suits[gmGetSuit(moves[i].card)].c_str(),
										gmUtil::m_values[gmGetValue(moves[i].card)].c_str(),
										moves[i].rank
									   ));
		} else {
			out.Append(wxString::Format(wxT("%s%s(%d),"),
										gmUtil::m_suits[gmGetSuit(moves[i].card)].c_str(),
										gmUtil::m_values[gmGetValue(moves[i].card)].c_str(),
										moves[i].rank
									   ));
		}
	}
	return out;
}

bool aiAgent::PostPlayUpdate(gmEngineData *data, int card) {
	int cards_left = 0;
	unsigned long cards_played = 0;
	int i;
	int suit;

	wxASSERT(card != gmCARD_INVALID);
	wxASSERT(data->status == gmSTATUS_TRICKS);

	// If the input card is valid
	if(card != gmCARD_INVALID) {

		suit = gmGetSuit(card);

		// If trump is not shown and if the player is the max bidder and
		// if it is the first card in the trick then the suit is not trump,
		// provided the max bidder has a choice of another suit
		if(
			!data->trump_shown &&
			(data->in_trick_info.player == data->curr_max_bidder)
		) {
			if(data->tricks[data->trick_round].count == 0 ) {
				// Get the count of cards belonging to the suit
				// already played
				for(i = 0; i < gmTOTAL_PLAYERS; i++) {
					cards_left += gmUtil::CountBitsSet(data->played_cards[i] & gmUtil::m_suit_mask[suit]);
				}
				// Add to that the number of cards held by the AI player
				// belonging to the suit
				cards_left += gmUtil::CountBitsSet(data->hands[m_loc] & gmUtil::m_suit_mask[suit]);
				// Consider the card being played
				cards_left++;

				// Check whether the number of cards left in the suit
				// is less than the number of cards left in max bidders hand
				// after the play
				// (8 - data->trick_round) > (8 - cards_left)
				if(data->trick_round < cards_left) {
					//wxLogDebug(wxString::Format("%s is not the trump", gmUtil::m_suits[suit].c_str()));
					m_trump_cards &= ~(((unsigned long)1) << suit);
				} else {
					m_notrump_suspects |= (((unsigned long)1) << suit);
				}
			}

			// Check each of the suspects, if the max bidder has played a card
			// which is not any of the suspects, the suspicion is valid
			if(m_notrump_suspects) {
				for(i = 0; i < gmTOTAL_SUITS; i++) {
					if((m_notrump_suspects & (((unsigned long)1) << i)) && (i != suit)) {
						// Remove the suit from the list of possible trump suits
						m_trump_cards &= ~(((unsigned long)1) << i);
						// Remove the suit from the list of suspects
						m_notrump_suspects &= ~(((unsigned long)1) << i);
					}
				}
			}
		}

		wxASSERT(m_trump_cards);

		// Check the cases where a player is unable to follow suit
		if(
			(data->tricks[data->trick_round].count > 0) &&
			(data->tricks[data->trick_round].lead_suit != suit)
		) {
			// If the current player is the max bidder
			// we can safely assume that either the suit is the trump
			// and the max bidder has only one card of the suit left and
			// that is kept as the trump. Otherwise the suit length is zero.
			// This can be confirmed only after the trump is shown
			if(
				(data->in_trick_info.player == data->curr_max_bidder) &&
				(!data->trump_shown)
			) {
				m_mb_null_susp |= (((unsigned long)1) << data->tricks[data->trick_round].lead_suit);
			}
			// If the player playing the card is not the max bidder
			// we can safely assume that the suit length for the player
			// is zero
			else {
				m_nulls[data->in_trick_info.player] |= (((unsigned long)1) << data->tricks[data->trick_round].lead_suit);
			}
		}
	}

	// If trump is shown and if any of the null suspects is not
	// the trump set the suit length in max bidders hand to zero
	// for those
	if((data->trump_shown) && (m_mb_null_susp)) {
		for(i = 0; i < gmTOTAL_SUITS; i++) {
			if((m_mb_null_susp & (((unsigned long)1) << i)) && (i != data->trump_suit)) {
				m_nulls[data->curr_max_bidder] |= (((unsigned long)1) << i);
				// Remove from the suspect list
				m_mb_null_susp &= ~(((unsigned long)1) << i);
			}
		}
	}

	// If self is not the max bidder and trump is not shown,
	// For any suit if the sum of the cards played(including the
	// the cards played in the current trick) and cards held by self
	// is 8, then the suit is not the trump.
	// This is because max bidder cannot have any card of the suit
	if((m_loc != data->curr_max_bidder) && (!data->trump_shown)) {
		cards_played = 0;

		// Add the card that is being played first
		if(card != gmCARD_INVALID) {
			cards_played |= (((unsigned long)1) << card);
		}

		// Add the cards played so far (previous tricks and the current one)
		for(i = 0; i < gmTOTAL_PLAYERS; i++) {
			cards_played |= data->played_cards[i];
			if(data->tricks[data->trick_round].cards[i] != gmCARD_INVALID)
				cards_played |= (((unsigned long)1) << data->tricks[data->trick_round].cards[i]);
		}

		for(i = 0; i < gmTOTAL_SUITS; i++) {
			if(gmUtil::CountBitsSet((cards_played | data->hands[m_loc]) &
									gmUtil::m_suit_mask[i])>= gmTOTAL_VALUES) {
				m_trump_cards &= ~(((unsigned long)1) << i);
			}
		}
	}

	return true;
}
bool aiAgent::CheckAssumptions(gmEngineData *data) {
	int i;

#ifdef raAI_LOG_CHECKASSUMPTIONS
#endif
	for(i = 0; i < gmTOTAL_SUITS; i++) {
		if(!(m_trump_cards & (((unsigned long)1) << i))) {
			if(data->trump_suit == i) {
				wxLogDebug(wxT("Dummy"));
			}
			wxASSERT(data->trump_suit != i);
		}
	}
	int j;
	for(j = 0; j < gmTOTAL_PLAYERS; j++) {
		for(i = 0; i < gmTOTAL_SUITS; i++) {
			if(m_nulls[j] & (((unsigned long)1) << i)) {
				wxASSERT(!(data->hands[j] & gmUtil::m_suit_mask[i]));
			}
		}
	}

	return true;
}

bool aiAgent::Reset() {
	int i;

	m_engine.Reset();
	m_trump_cards = 0x0000000F;
	for(i = 0; i < gmTOTAL_PLAYERS; i++)
		m_nulls[i] = 0;
	m_notrump_suspects = 0;
	m_mb_null_susp = 0;
	return true;
}

void aiAgent::SetRules(pgmRules rules) {
	gmEngineData data;
	if(rules) {
		m_engine.GetData(&data);

		data.rules = *rules;

		m_engine.SetData(&data, false);
	}
}

bool aiAgent::SetClockwise(bool flag) {
	gmEngineData data;
	m_engine.GetData(&data);
	if(flag)
		data.rules.rot_addn = 1;
	else
		data.rules.rot_addn = 3;
	m_engine.SetData(&data, false);

	return true;
}
bool aiAgent::GetClockwise() {
	gmEngineData data;
	m_engine.GetData(&data);
	switch(data.rules.rot_addn) {
	case 1:
		return true;
		break;
	case 3:
		return false;
		break;
	default:
		return false;
		break;
	}

	return false;
}
bool aiAgent::AbandonGame(bool *flag) {
	wxASSERT(flag);
	// TODO : Add more intelligent logic
	*flag = true;
	return true;
}


//
// Private method/s
//
bool aiAgent::EstimateTricks(unsigned long *p_hands, int trump, int *eval) {
	int i, j, k; // Multi-purpose counters
	unsigned long combined[2];
	unsigned long suit[2];
	int stronger, tricks[2];
	int suit_count[4];
	int trump_adv;

	// Initialization
	tricks[0] = 0;
	tricks[1] = 0;


	// Combine each teams hands
	combined[0] = p_hands[0] | p_hands[2];
	combined[1] = p_hands[1] | p_hands[3];

#ifdef raAI_LOG_ESTIMATE_TRICKS
	wxLogDebug("----------------------------------------------------");

	wxLogDebug(gmUtil::PrintHands(p_hands));
	wxLogDebug(_("Trump is ") + gmUtil::m_suits[trump]);
	wxLogDebug("Combined hands :");
	wxLogDebug(_("N/S - ") + gmUtil::PrintLong(combined[0]));
	wxLogDebug(_("E/W - ") + gmUtil::PrintLong(combined[1]));
#endif

	// Loop though each of the suits
	for(i = 0; i < 4; i++) {
		suit[0] = (combined[0] & gmUtil::m_suit_mask[i]) >> gmUtil::m_suit_rs[i];
		suit[1] = (combined[1] & gmUtil::m_suit_mask[i]) >> gmUtil::m_suit_rs[i];
#ifdef raAI_LOG_ESTIMATE_TRICKS
		wxLogDebug("-----------------------");
		wxLogDebug(_("Evaluating suit - ") + gmUtil::m_suits[i]);
#endif
		if(suit[0] > suit[1])
			j = 0;
		else
			j = 1;

#ifdef raAI_LOG_ESTIMATE_TRICKS
		wxLogDebug(_("Stronger team is - ") + gmUtil::m_short_teams[j]);
#endif

		// Number of cards with the stronger team which are stronger than
		// the strongest card in the weakest team

		stronger = gmUtil::CountBitsSet(
					   suit[j] & (0xFFFFFFFF << gmUtil::HighestBitSet(suit[!j])));
#ifdef raAI_LOG_ESTIMATE_TRICKS
		wxLogDebug(wxString::Format("Number of stronger cards - %d", stronger));
#endif

		suit_count[!j] =
			gmUtil::CountBitsSet(p_hands[!j] & gmUtil::m_suit_mask[i]);
		suit_count[(!j) + 2] =
			gmUtil::CountBitsSet(p_hands[(!j) + 2] & gmUtil::m_suit_mask[i]);
#ifdef raAI_LOG_ESTIMATE_TRICKS
		wxLogDebug(_("Individual counts of cards with opposition ") + gmUtil::m_short_teams[!j]);
		wxLogDebug(wxString::Format("%s - %d", gmUtil::m_long_locs[!j].c_str(), suit_count[!j]));
		wxLogDebug(wxString::Format("%s - %d", gmUtil::m_long_locs[(!j) + 2].c_str(), suit_count[(!j) + 2]));
#endif

		// If the suit is not the trump suit,
		//   take the minimum of stronger cards and the number of cards
		//   with each of the weaker locations. This is to accommodate
		//   chances of getting trumped
		// If the suit is the trump suit,
		//   the number of tricks expected is the sum of
		//   the number of stronger cards
		//   and the difference of maximum number of cards

		if(i == trump) {
			suit_count[j] =
				gmUtil::CountBitsSet(p_hands[j] & gmUtil::m_suit_mask[i]);
			suit_count[j + 2] =
				gmUtil::CountBitsSet(p_hands[j + 2] & gmUtil::m_suit_mask[i]);

#ifdef raAI_LOG_ESTIMATE_TRICKS
			wxLogDebug(_("Tricks expected - ") +
					   wxString::Format("%d", std::min(stronger, std::max(suit_count[j], suit_count[j + 2]))));
#endif
			tricks[j] += std::min(stronger, std::max(suit_count[j], suit_count[j + 2]));

			//
			// Calculating trump advantage
			//
			// Remove cards from each of the hands, equal to the number of
			// tricks expected. If there is any advantage in the suit length
			// after this, that is considered
			for(k = 0; k < 4; k++) {
				// Is this the best way to do this sort of negation?
				suit_count[k] -= stronger;
				if(suit_count[k] < 0)
					suit_count[k] = 0;
			}

			trump_adv = std::max(suit_count[j], suit_count[j + 2])
						- std::max(suit_count[!j], suit_count[(!j) + 2]);
#ifdef raAI_LOG_ESTIMATE_TRICKS
			wxLogDebug(_("Trump advantage for ") + gmUtil::m_short_teams[j].c_str() + _(" is ") + wxString::Format("%d", trump_adv));
#endif
			if(trump_adv > 0)
				tricks[j] += trump_adv;
			else
				tricks[!j] -= trump_adv;
		} else {
#ifdef raAI_LOG_ESTIMATE_TRICKS
			wxLogDebug(_("Tricks expected - ") +
					   wxString::Format("%d", std::min(std::min(suit_count[!j], suit_count[(!j) + 2]), stronger)));
#endif
			tricks[j] += std::min(std::min(suit_count[!j], suit_count[(!j) + 2]), stronger);
		}
	}
	// If either is greater that 8 correct
	if(tricks[0] > 8)
		tricks[0] = 8;
	if(tricks[1] > 8)
		tricks[1] = 8;

#ifdef raAI_LOG_ESTIMATE_TRICKS
	wxLogDebug(wxString::Format("Total no of tricks expected %d, %d", tricks[0], tricks[1]));
	wxLogDebug("----------------------------------------------------");
#endif
	eval[0] = tricks[0];
	eval[1] = tricks[1];
	return true;
}
bool aiAgent::EstimatePoints(unsigned long *hands, int trump, int trick_no, int *eval) {
	int trick_count[2];
	unsigned long all_cards = 0;
	int total_pts;

	// Set both ints in eval to 0
	for (int i=0; i<2; i++) {
		eval [i]=0;
	}

	all_cards =  hands[0] | hands[1] | hands[2] | hands[3];
	total_pts = gmTotalPoints(all_cards);

	EstimateTricks(hands, trump, trick_count);

	// Share points according to the tricks estimated

	eval[0] = (total_pts * trick_count[0]) / (8 - trick_no);
	eval[1] = (total_pts * trick_count[1]) / (8 - trick_no);
#ifdef  raAI_LOG_ESTIMATE_POINTS
	wxLogDebug(wxString::Format("Points expected before sharing - %d, %d", eval[0], eval[1]));
	wxLogDebug(wxString::Format("Points shared- %d", (total_pts - eval[0] - eval[1]) / 4));
#endif

	// Share the half of the rest of the points equally
	eval[0] += (total_pts - eval[0] - eval[1]) / 4;
	eval[1] += (total_pts - eval[0] - eval[1]) / 4;

#ifdef  raAI_LOG_ESTIMATE_POINTS
	wxLogDebug(wxString::Format("Points expected (final) - %d, %d", eval[0], eval[1]));
#endif

	return true;
}

bool aiAgent::GenerateMoves(gmEngine *node, aiMove *moves, int *count, int type) {
	gmInputTrickInfo trick_info;
	int i = 0;
	unsigned long cards_left;
	unsigned long hands[gmTOTAL_PLAYERS];
	gmEngine rule_engine;
	gmEngineData re_data;
#ifdef raAI_LOG_GENERATEMOVES
	wxString out;
#endif

	wxASSERT(moves);
	wxASSERT(count);

	// Check whether the rule engine is expecting a
	// card to be played
	if(node->GetPendingInputType() != gmINPUT_TRICK) {
		wxLogError(wxString::Format(wxT("Trick not expected. %s:%d"),
									wxT(__FILE__), __LINE__));
		return false;
	}

	// Get the input criteria for future use
	if(!node->GetPendingInputCriteria(NULL, &trick_info)) {
		wxLogError(wxString::Format(wxT("GetPendingInputCriteria failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return false;
	}

	// Depending on the type passed, generate
	// moves which do not ask for the trump

	if(type & aiGENMV_NOTRUMP) {
		// Get the cards in the hand and generate all possible card plays
		// which can be played without asking for trump
		node->GetHands(hands);
		cards_left = hands[trick_info.player];
		cards_left &= trick_info.mask;

		while(cards_left) {
			moves[i].ask_trump = false;
			moves[i].card = gmUtil::HighestBitSet(cards_left);
			moves[i].rank = -100;
			cards_left &= ~(((unsigned long)1) << moves[i].card);
			i++;
		}
	}
	// Depending on the type passed, generate moves
	// that ask for the trump
	if(type & aiGENMV_TRUMP) {
		// If trump can be asked, generate all possible card plays
		// than can be made after asking for the trump

		if(trick_info.can_ask_trump) {
			node->GetData(&re_data);
			rule_engine.SetData(&re_data, false);

			trick_info.ask_trump = true;
			if(rule_engine.PostInputMessage(gmINPUT_TRICK, &trick_info)) {
				wxLogError(wxString::Format(wxT("PostInputMessage failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				return false;
			}
			while(rule_engine.Continue());

			if(rule_engine.GetPendingInputType() != gmINPUT_TRICK) {
				wxLogError(wxString::Format(wxT("Trick not expected. %s:%d"),
											wxT(__FILE__), __LINE__));
				return false;
			}

			// Get the input criteria
			if(!rule_engine.GetPendingInputCriteria(NULL, &trick_info)) {
				wxLogError(wxString::Format(wxT("GetPendingInputCriteria failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				return false;
			}

			rule_engine.GetHands(hands);
			cards_left = hands[trick_info.player];
			cards_left &= trick_info.mask;

			while(cards_left) {
				moves[i].ask_trump = true;
				moves[i].card = gmUtil::HighestBitSet(cards_left);
				moves[i].rank = -100;
				cards_left &= ~(((unsigned long)1) << moves[i].card);
				i++;
			}

		}
	}

	wxASSERT(i <= aiMAX_MOVES);
	*count = i;

	// TODO : Use PrintMoves()
#ifdef raAI_LOG_GENERATEMOVES
	out.Append(wxString::Format("%d moves generated for - ", *count));
	for(i = 0; i < *count; i++) {
		if(moves[i].ask_trump) {
			out.Append(wxString::Format("?%s%s,",
										gmUtil::m_suits[gmGetSuit(moves[i].card)].c_str(),
										gmUtil::m_values[gmGetValue(moves[i].card)].c_str()
									   ));
		} else {
			out.Append(wxString::Format("%s%s,",
										gmUtil::m_suits[gmGetSuit(moves[i].card)].c_str(),
										gmUtil::m_values[gmGetValue(moves[i].card)].c_str()
									   ));
		}
	}
	wxLogDebug(out);
#endif

	return true;
}
bool aiAgent::OrderMoves(gmEngine *node, aiMove *moves, int count) {
	int i;
	gmEngineData data;

	node->GetData(&data);

	// Rank each move
	for(i = 0; i < count; i++)
		RankMove(&data, &moves[i]);

	qsort(moves, count, sizeof(aiMove), CompareMoves);

	return true;
}
bool aiAgent::RankMove(gmEngineData *data, aiMove *move) {
	int next_to_play = gmPLAYER_INVALID;
	unsigned long trick_cards = 0;
	int i;
	gmTrick *trick;
	int suit;
	int value;
	int lead_suit;

	unsigned long opp1_cards_suit = 0, opp2_cards_suit = 0, partner_cards_suit = 0;
	unsigned long opp1_cards_trump = 0, opp2_cards_trump = 0, partner_cards_trump = 0;
	unsigned long ucard;
	unsigned long trick_cards_suit = 0, trick_cards_trump = 0;
	bool opp1_played = false, opp2_played = false, partner_played = false;

	wxASSERT(data);
	wxASSERT(move);

	wxASSERT(data->status == gmSTATUS_TRICKS);
	next_to_play = data->in_trick_info.player;
	wxASSERT((next_to_play > gmPLAYER_INVALID) && (next_to_play < gmTOTAL_PLAYERS));
	trick = &data->tricks[data->trick_round];
	wxASSERT(trick);
	suit = gmGetSuit(move->card);
	wxASSERT((suit > gmSUIT_INVALID) && (suit < gmTOTAL_SUITS));
	value = gmGetValue(move->card);
	ucard = (((unsigned long)1) << move->card);
	wxASSERT(ucard);

	if(!trick->count)
		lead_suit = suit;
	else
		lead_suit = trick->lead_suit;

	opp1_cards_suit = data->hands[gmGetOpponentOne(next_to_play)] &
					  gmUtil::m_suit_mask[lead_suit];
	opp2_cards_suit = data->hands[gmGetOpponentTwo(next_to_play)] &
					  gmUtil::m_suit_mask[lead_suit];
	opp1_cards_trump = data->hands[gmGetOpponentOne(next_to_play)] &
					   gmUtil::m_suit_mask[data->trump_suit];
	opp2_cards_trump = data->hands[gmGetOpponentTwo(next_to_play)] &
					   gmUtil::m_suit_mask[data->trump_suit];

	partner_cards_suit = data->hands[gmGetPartner(next_to_play)] &
						 gmUtil::m_suit_mask[lead_suit];
	partner_cards_trump = data->hands[gmGetPartner(next_to_play)] &
						  gmUtil::m_suit_mask[data->trump_suit];

	// Combine all the cards played so far in the trick
	for(i = 0; i < gmTOTAL_PLAYERS; i++) {
		if(trick->cards[i] != gmCARD_INVALID) {
			trick_cards |= (((unsigned long)1) << trick->cards[i]);
		}
	}

	trick_cards_suit = trick_cards & gmUtil::m_suit_mask[lead_suit];
	trick_cards_trump = trick_cards & gmUtil::m_suit_mask[data->trump_suit];

	if(trick->cards[gmGetPartner(next_to_play)] != gmCARD_INVALID)
		partner_played = true;
	if(trick->cards[gmGetOpponentOne(next_to_play)] != gmCARD_INVALID)
		opp1_played = true;
	if(trick->cards[gmGetOpponentTwo(next_to_play)] != gmCARD_INVALID)
		opp2_played = true;

	// Is the move the highest card possible for the suit
	// and can it take the trick?
	// Or can the partner do the same?
	// If the trick is not already trumped
	if(
		(!trick->trumped || (trick->lead_suit == data->trump_suit))
	) {
		// Can the move take the trick by playing
		// the highest card of the lead suit?
		if(
			// And if the card played is higher than the cards played
			// in the trick belonging to the same suit
			(ucard > trick_cards_suit) &&
			// And if the card is the first to be played in the trick
			// or if the card played has the same suit as the lead suit
			(suit == lead_suit) &&
			//((!trick->count) || ((trick->count > 0) && (trick->lead_suit == suit))) &&
			// And if opponent has not played yet and the opponent has the suit but no higher cards
			// or if opponent one has no cards belonging to the suit but no trumps
			(opp1_played || (!opp1_played && ((opp1_cards_suit && (opp1_cards_suit < ucard)) || (!opp1_cards_suit && !opp1_cards_trump)))) &&
			(opp2_played || (!opp2_played && ((opp2_cards_suit && (opp2_cards_suit < ucard)) || (!opp2_cards_suit && !opp2_cards_trump))))
		) {
			move->rank = 400 + gmUtil::m_points[value];
			return true;
		}
		// Can the partner take the trick by playing the highest
		// card in the lead suit
		else if (
			// And if the card played is higher than the cards played
			// in the trick belonging to the same suit
			((trick->winner == gmGetPartner(next_to_play)) || (!partner_played && (partner_cards_suit > trick_cards_suit))) &&
			// And if the move is not the trump card
			// TODO : Implement - wisely!
			// And if opponent has not played yet and the opponent has the suit but no higher cards
			// or if opponent one has no cards belonging to the suit but no trumps
			(opp1_played || (!opp1_played && ((opp1_cards_suit && (opp1_cards_suit < partner_cards_suit)) || (!opp1_cards_suit && !opp1_cards_trump)))) &&
			(opp2_played || (!opp2_played && ((opp2_cards_suit && (opp2_cards_suit < partner_cards_suit)) || (!opp2_cards_suit && !opp2_cards_trump))))
		) {
			move->rank = 300 + gmUtil::m_points[value];
			return true;
		}
	}

	// Can the move trump the trick and win it?
	if(
		trick->count && move->ask_trump && (suit == data->trump_suit) &&
		// The trick is not already trumped or you can over-trump
		(!trick->trumped || (ucard > trick_cards_trump)) &&
		// Opponent has not played or opponent has the lead suit or opponent does not have
		// the lead suit but cannot over-trump
		(opp1_played || (!opp1_played && (opp1_cards_suit || (!opp1_cards_suit && (opp1_cards_trump < ucard))))) &&
		(opp2_played || (!opp2_played && (opp2_cards_suit || (!opp2_cards_suit && (opp2_cards_trump < ucard)))))
	) {
		move->rank = 200 - gmUtil::m_points[value];
		return true;
	}

	bool partner_trumps = false;
	// If partner has already trumped and the opponents cannot over-trump
	if(
		trick->trumped && (trick->winner == gmGetPartner(next_to_play))
	) {
		if(
			(opp1_played || (!opp1_played && (opp1_cards_suit || (!opp1_cards_suit && (opp1_cards_trump < (unsigned long)(((unsigned long)1) << trick->cards[trick->winner])))))) &&
			(opp2_played || (!opp2_played && (opp2_cards_suit || (!opp2_cards_suit && (opp2_cards_trump < (unsigned long)(((unsigned long)1) << trick->cards[trick->winner]))))))
		) {
			partner_trumps = true;
		}
	}

	// If partner has not played yet
	if(!partner_played) {
		// If trick is already trumped, and if partner has a bigger trump
		// and if opponents cannot over-trump
		if(
			(
				(trick->trumped && (partner_cards_trump > (unsigned long)(((unsigned long)1) << trick->cards[trick->winner]))) ||
				(!trick->trumped && partner_cards_trump && !partner_cards_suit)
			)&&
			(opp1_played || (!opp1_played && (opp1_cards_suit || (!opp1_cards_suit && (opp1_cards_trump < partner_cards_trump))))) &&
			(opp2_played || (!opp2_played && (opp2_cards_suit || (!opp2_cards_suit && (opp2_cards_trump < partner_cards_trump)))))
		) {
			partner_trumps = true;
		}
	}

	if(partner_trumps) {
		move->rank = 100 + gmUtil::m_points[value];
		return true;
	}

	move->rank = 0 - gmUtil::m_points[value];
	return true;
}

/*
* Alpha-Beta search...
* Pseudo code -
evaluate (node, alpha, beta)
	if node is a leaf
		return the heuristic value of node
	if node is a minimizing node
		for each child of node
			beta = min (beta, evaluate (child, alpha, beta))
			if beta <= alpha
				return alpha
			return beta
	if node is a maximizing node
		for each child of node
			alpha = max (alpha, evaluate (child, alpha, beta))
			if beta <= alpha
				return beta
			return alpha
*
*/
int aiAgent::Evaluate(gmEngine *node, int alpha, int beta, int depth, bool *ret_val) {
	int eval;
	gmEngineData old_node;
	int i;
	int trick_round;
	gmInputTrickInfo trick_info;
	aiMove moves[aiMAX_MOVES];
	int move_count;
	int ret_heur;

	wxASSERT(ret_val);
	*ret_val = true;

	trick_round = node->GetTrickRound();
	wxASSERT((trick_round >= 0) && (trick_round <= 8));

#ifdef raAI_LOG_EVALUATE
	wxLogDebug(wxString::Format(wxT("Evaluating round %d. %s:%d"),
								trick_round, wxT(__FILE__), __LINE__));
#endif

	// If node is leaf, estimate heuristic
	if((trick_round == 8) || (trick_round >= depth)) {
		ret_heur = EstimateHeuristic(node);
#ifdef raAI_LOG_EVALUATE
		wxLogDebug("Logging at leaf");
		wxLogDebug("-------------------------------");
		wxLogDebug(node->GetLoggable());
		wxLogDebug(wxString::Format("Estimated Heuristic - %d", ret_heur));
		wxLogDebug("-------------------------------");
#endif
		return ret_heur;
	}

	// Backup the current state of the rule engine
	node->GetData(&old_node);

	if(node->GetPendingInputType() != gmINPUT_TRICK) {
		wxLogError(wxString::Format(wxT("Unexpected input type. %s:%d"),
									wxT(__FILE__), __LINE__));
		*ret_val = false;
		return 0;
	}

	if(!node->GetPendingInputCriteria(NULL, &trick_info)) {
		wxLogError(wxString::Format(wxT("GetPendingInputCriteria failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		*ret_val = false;
		return 0;
	}

	//if node is a minimizing node
	if((trick_info.player & 1) != (m_loc & 1)) {
		GenerateMoves(node, moves, &move_count);
		wxASSERT(move_count);
		//wxLogDebug("Before ordering");
		//wxLogDebug(PrintMoves(moves, move_count));
#ifdef raAI_ORDERMOVES
		if(move_count > 1)
			OrderMoves(node, moves, move_count);
#endif
		//wxLogDebug("After ordering");
		//wxLogDebug(PrintMoves(moves, move_count));
		if(move_count <= 0) {
			wxLogError(node->GetLoggable());
		}
		// for each child of node
		for(i = 0; i < move_count; i++) {
			//::wxYield();
			if(!MakeMove(node, &moves[i])) {
				wxLogError(wxString::Format(wxT("MakeMove failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				*ret_val = false;
				return 0;
			}
			//beta = min (beta, evaluate (child, alpha, beta))
			eval =  Evaluate(node, alpha, beta, depth, ret_val);
			if(!*ret_val) {
				wxLogError(wxString::Format(wxT("Evaluate failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				return 0;
			}
			beta = std::min(beta, eval);
			node->SetData(&old_node, false);
			//if beta <= alpha
			//	return alpha
			if(beta <= alpha) {
				return alpha;
			}
		}
		//return beta
		return beta;

	}
	//if node is a maximizing node
	else {
		GenerateMoves(node, moves, &move_count);
		wxASSERT(move_count);
		//wxLogDebug("Before ordering");
		//wxLogDebug(PrintMoves(moves, move_count));
#ifdef  raAI_ORDERMOVES
		if(move_count > 1)
			OrderMoves(node, moves, move_count);
#endif
		//wxLogDebug("After ordering");
		//wxLogDebug(PrintMoves(moves, move_count));
		if(move_count <= 0) {
			wxLogError(node->GetLoggable());
		}
		// for each child of node
		for(i = 0; i < move_count; i++) {
			//::wxYield();
			if(!MakeMove(node, &moves[i])) {
				wxLogError(wxString::Format(wxT("MakeMove failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				*ret_val = false;
				return 0;
			}
			//alpha = max (alpha, evaluate (child, alpha, beta))
			eval = Evaluate(node, alpha, beta, depth, ret_val);
			if(!*ret_val) {
				wxLogError(wxString::Format(wxT("Evaluate failed. %s:%d"),
											wxT(__FILE__), __LINE__));
				return 0;
			}
			alpha = std::max(alpha, eval);
			node->SetData(&old_node, false);
			//if beta <= alpha
			//    return beta
			if(beta <= alpha) {
				return beta;
			}
		}
		// return alpha
		return alpha;
	}

	return 0;
}

int aiAgent::EstimateHeuristic(gmEngine *state) {
	int pts[2];
	int estimated[2];
	int trick_no;
	int bid;
	int loc;

	//unsigned long played_cards[gmTOTAL_PLAYERS];
	unsigned long hands[gmTOTAL_PLAYERS];

	trick_no = state->GetTrickRound();
	state->GetMaxBid(&bid, &loc);
	state->GetPoints(pts);

	if(trick_no == 8) {
		// If the maximum bid was made a player from the same team
		if((loc & 1) == (m_loc & 1)) {
			return ((pts[m_loc & 1] * (29 -bid)) -  (pts[!(m_loc & 1)] * bid));
		} else {
			return ((pts[m_loc & 1] * bid) -  (pts[!(m_loc & 1)] * (29 - bid)));
		}
	} else {
		state->GetHands(hands);
		EstimatePoints(hands, state->GetTrump(), trick_no, estimated);
		if((loc & 1) == (m_loc & 1)) {
			return ((estimated[m_loc & 1] + pts[m_loc & 1]) * (29 - bid)) -
				   ((estimated[!(m_loc & 1)] + pts[!(m_loc & 1)]) * bid);
		} else {
			return ((estimated[m_loc & 1] + pts[m_loc & 1]) * bid) -
				   ((estimated[!(m_loc & 1)] + pts[!(m_loc & 1)]) * (29 - bid));
		}
	}
	return 0;
}

bool aiAgent::MakeMove(gmEngine *node, aiMove *move) {
	gmInputTrickInfo trick_info;

	// Obtain the current input criteria and verify

	if(node->GetPendingInputType() != gmINPUT_TRICK) {
		wxLogError(wxString::Format(wxT("GetPendingInputCriteria failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return false;
	}

	if(!node->GetPendingInputCriteria(NULL, &trick_info)) {
		wxLogError(wxString::Format(wxT("GetPendingInputCriteria failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return false;
	}

	// If trump needs to be asked for firstly, do that
	if(move->ask_trump) {
		wxASSERT(trick_info.can_ask_trump);
		trick_info.ask_trump = true;
		if(node->PostInputMessage(gmINPUT_TRICK, &trick_info)) {
			wxLogError(wxString::Format(wxT("PostInputMessage failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return false;
		}
		while(node->Continue());

		if(node->GetPendingInputType() != gmINPUT_TRICK) {
			wxLogError(wxString::Format(wxT("GetPendingInputCriteria failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return false;
		}

		if(!node->GetPendingInputCriteria(NULL, &trick_info)) {
			wxLogError(wxString::Format(wxT("GetPendingInputCriteria failed. %s:%d"),
										wxT(__FILE__), __LINE__));
			return false;
		}
	}

	// Make the move
	trick_info.card = move->card;

#ifdef raAI_LOG_MAKEMOVE
	wxLogDebug(wxString::Format(wxT("%s making a move %s%s. %s:%d"),
								gmUtil::m_short_locs[trick_info.player].c_str(),
								gmUtil::m_suits[gmGetSuit(move->card)].c_str(),
								gmUtil::m_values[gmGetValue(move->card)].c_str(),
								wxT(__FILE__), __LINE__));
#endif
	if(node->PostInputMessage(gmINPUT_TRICK, &trick_info)) {
		node->PostInputMessage(gmINPUT_TRICK, &trick_info);
		wxLogDebug(wxString::Format(wxT("Player %s Card %s%s"),
									gmUtil::m_short_locs[trick_info.player].c_str(),
									gmUtil::m_suits[gmGetSuit(trick_info.card)].c_str(),
									gmUtil::m_values[gmGetValue(trick_info.card)].c_str()
								   ));
		wxLogError(wxString::Format(wxT("PostInputMessage failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return false;
	}

	// Continue the play
	while(node->Continue());

	return true;
}

bool aiAgent::MakeMoveAndEval(gmEngine *node, aiMove *move, int depth, int *eval) {
	bool eval_ret;
	int temp;

	wxASSERT(node);
	wxASSERT(eval);
	wxASSERT(move);
	wxASSERT(depth > 0);

	if(!MakeMove(node, move)) {
		wxLogError(wxString::Format(wxT("MakeMove failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		wxLogError(node->GetLoggable());
		if(move->ask_trump) {
			wxLogError(wxString::Format(wxT("Move attempted ?%s%s"),
										gmUtil::m_suits[gmGetSuit(move->card)].c_str(),
										gmUtil::m_values[gmGetValue(move->card)].c_str()
									   ));
		} else {
			wxLogError(wxString::Format(wxT("Move attempted %s%s"),
										gmUtil::m_suits[gmGetSuit(move->card)].c_str(),
										gmUtil::m_values[gmGetValue(move->card)].c_str()
									   ));
		}
		return false;
	}

	eval_ret = false;
	temp = aiNEG_INFTY;
	temp = Evaluate(node, aiNEG_INFTY, aiPOS_INFTY, depth, &eval_ret);
	wxASSERT(temp != aiNEG_INFTY);
	if(!eval_ret) {
		wxLogError(wxString::Format(wxT("Evaluate failed. %s:%d"),
									wxT(__FILE__), __LINE__));
		return false;
	}

	*eval = temp;

	return true;
}

int aiAgent::CompareMoves(const void *elem1, const void *elem2) {
	aiMove *move1, *move2;
	move1 = (aiMove *)elem1;
	move2 = (aiMove *)elem2;
	if(move1->rank > move2->rank)
		return -1;
	else if(move1->rank == move2->rank)
		return 0;
	else
		return 1;
}
