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
#include "gmEngineData.h"

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

typedef gmRules* pgmRules;

#define gmNext(X) ((X + m_data.rules.rot_addn) % gmTOTAL_PLAYERS)
#define gmTrickNext ((m_data.tricks[m_data.trick_round].lead_loc + (m_data.tricks[m_data.trick_round].count * m_data.rules.rot_addn)) % 4)
#define gmWinnerCard (m_data.tricks[m_data.trick_round].cards[m_data.tricks[m_data.trick_round].winner])

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
