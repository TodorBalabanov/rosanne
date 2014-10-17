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

#ifndef _RAPLAYER_H_
#define _RAPLAYER_H_

#include "racommon.h"
#include "aiagent.h"

enum {
	raPLAYER_TYPE_INVALID = -1,
	raPLAYER_TYPE_HUMAN,
	raPLAYER_TYPE_AI
};

class raPlayer {
private:
	aiAgent m_agent;
	int m_loc;
	int m_type;
	int m_trump;
public:
	raPlayer();
	~raPlayer();
	void SetLocation(int loc);
	int GetLocation();
	int GetType();
	void SetType(int type);
	void SetRuleEngineData(gmEngineData *data);
	bool GetBid(int *bid, int *trump, int min, bool force_bid);
	int GetTrump();
	int GetPlay();
	bool PostPlayUpdate(gmEngineData *data, int card);
	bool CheckAssumptions(gmEngineData *data);
	bool Reset();
	void SetRules(pgmRules rules = NULL);
	bool SetClockwise(bool flag);
	bool GetClockwise();
	bool AbandonGame(bool *flag);
};

#endif
