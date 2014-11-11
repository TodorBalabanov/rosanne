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

#ifndef _AIAGENT_H_
#define _AIAGENT_H_

#include "racommon.h"
#include "gmengine.h"
#include "aisuitlengthsolver.h"
#include "gmutil.h"

#include "aiMove.h"
#include "slProblem.h"

#define aiBID_SAMPLE 100
#define aiPLAY_SAMPLES 30
#define aiMAX_MOVES 20
#define aiGENMV_NOTRUMP 1
#define aiGENMV_TRUMP 2
#define aiGENMV_ALL 3

#define aiPOS_INFTY +10000
#define aiNEG_INFTY -10000

class aiAgent {
private:
	// Disallow copy constructor/assignment operators
	aiAgent(const aiAgent &);
	aiAgent & operator=(const aiAgent &);
	gmEngine m_engine;
	int m_loc;
	unsigned long m_trump_cards;
	unsigned long m_notrump_suspects;
	unsigned long m_nulls[gmTOTAL_PLAYERS];
	unsigned long m_mb_null_susp;
	bool EstimateTricks(unsigned long *p_hands, int trump, int *eval);
	bool EstimatePoints(unsigned long *hands, int trump, int trick_count, int *eval);
	bool GenerateMoves(gmEngine *node, aiMove *moves, int *count, int type = aiGENMV_ALL);
	bool OrderMoves(gmEngine *node, aiMove *moves, int count);
	bool RankMove(gmEngineData *data, aiMove *move);
	int Evaluate(gmEngine *node, int alpha, int beta, int depth, bool *ret_val);
	int EstimateHeuristic(gmEngine *state);
	bool MakeMove(gmEngine *node, aiMove *move);
	bool MakeMoveAndEval(gmEngine *node, aiMove *move, int depth, int *eval);
	const static int s_depths[];
	static int CompareMoves(const void *elem1, const void *elem2);
public:
	aiAgent();
	~aiAgent();
	void SetLocation(int loc);
	int GetLocation();
	bool GetBid(unsigned long cards, int *bid, int *trump, int min, bool force_bid);
	bool GetBid(int *bid, int *trump, int min, bool force_bid);
	bool SetRuleEngineData(gmEngineData *data);
	int GetTrump();
	static int GetTrump(unsigned long hand, int suit);
	int GetPlay(unsigned long mask);
	bool GenerateSLProblem(gmEngineData *data, slProblem *problem, slPlayed played, int trump, bool *add_trump);
	//bool GenerateSLSolution(slProblem *problem, slSolution *solution);
	bool GenerateDeals(gmEngineData *data, unsigned long **deals, int count, int trump = gmSUIT_INVALID);
	static wxString PrintMoves(aiMove *moves, int move_count);
	bool PostPlayUpdate(gmEngineData *data, int card = gmCARD_INVALID);
	bool CheckAssumptions(gmEngineData *data);
	bool Reset();
	void SetRules(pgmRules rules = NULL);
	bool SetClockwise(bool flag);
	bool GetClockwise();
	bool AbandonGame(bool *flag);

};

#endif
