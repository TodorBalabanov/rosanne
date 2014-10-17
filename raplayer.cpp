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

#include "raplayer.h"

raPlayer::raPlayer() {
	// TODO : Remove hard coding
	m_loc = 0;
	m_type = raPLAYER_TYPE_HUMAN;
	m_trump = gmSUIT_INVALID;
}

raPlayer::~raPlayer() {
}
void raPlayer::SetLocation(int loc) {
	wxASSERT((loc >= 0) && (loc < gmTOTAL_PLAYERS));
	m_loc = loc;
	m_agent.SetLocation(m_loc);
}
int raPlayer::GetLocation() {
	return m_loc;
}
int raPlayer::GetType() {
	return m_type;
}
void raPlayer::SetType(int type) {
	m_type = type;
}
void raPlayer::SetRuleEngineData(gmEngineData *data) {
	m_agent.SetRuleEngineData(data);
}
bool raPlayer::GetBid(int *bid, int *trump, int min, bool force_bid) {
	bool ret_val;
	ret_val = m_agent.GetBid(bid, trump, min, force_bid);
	m_trump = *trump;
	return ret_val;
}
int raPlayer::GetTrump() {
	return m_agent.GetTrump();
}
int raPlayer::GetPlay() {
	return m_agent.GetPlay(0);
}

bool raPlayer::PostPlayUpdate(gmEngineData *data, int card) {
	return m_agent.PostPlayUpdate(data, card);
}
bool raPlayer::CheckAssumptions(gmEngineData *data) {
	return m_agent.CheckAssumptions(data);
}
bool raPlayer::Reset() {
	return m_agent.Reset();
}
void raPlayer::SetRules(pgmRules rules) {
	m_agent.SetRules(rules);
}
bool raPlayer::SetClockwise(bool flag) {
	return m_agent.SetClockwise(flag);
}
bool raPlayer::GetClockwise() {
	return m_agent.GetClockwise();
}
bool raPlayer::AbandonGame(bool *flag) {
	return m_agent.AbandonGame(flag);
}



