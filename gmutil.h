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


#ifndef _GMUTIL_H_
#define _GMUTIL_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#define SPACES20 _("                    ")

#define gmTOTAL_CARDS 32
#define gmTOTAL_PLAYERS 4
#define gmTOTAL_BID_ROUNDS 3
#define gmPLAYER_INVALID -1
#define gmSUIT_INVALID -1
#define gmCARD_INVALID -1
#define gmPartner(X) ((X + 2) % gmTOTAL_PLAYERS)

#define gmBID_PASS 0
#define gmBID_ALL 1000
#define gmTOTAL_SUITS 4
#define gmTOTAL_VALUES 8
#define gmTOTAL_TRICKS 8
#define gmTOTAL_TEAMS 2

#define gmGetSuit(X) (X / gmTOTAL_VALUES)
#define gmGetValue(X) (X % gmTOTAL_VALUES)
#define gmGetTeam(X) (X % gmTOTAL_TEAMS)
#define gmGetOpponent(X) ((X + 1) % gmTOTAL_TEAMS)
#define gmGetPartner(X) ((X + 2) % gmTOTAL_PLAYERS)
#define gmGetOpponentOne(X) ((X + 1) % gmTOTAL_PLAYERS)
#define gmGetOpponentTwo(X) ((X + 3) % gmTOTAL_PLAYERS)

class gmUtil
{
public:
	static int m_value_trans[];
	static void ShuffleArray(int *array, unsigned long n);
	static wxString PrintLong(unsigned long cards);
	static wxString PrintHands(unsigned long *hands);
	static wxString m_suits[];
	static wxString m_values[];
	static wxString m_short_locs[];
	static wxString m_long_locs[];
	static unsigned long m_suit_mask[];
	static unsigned long m_suit_rs[];
	static int m_points[];
	static int m_total_points[];
	static wxString m_short_teams[];
	static int GetCardIndex(wxString text);
	static bool SetStatusText(const wxString& text, int i = 0);
	static unsigned int BitsSetTable256[];
	static int LogTable256[];
	static unsigned long CountBitsSet(unsigned long v);
	static unsigned long HighestBitSet(unsigned long v);
};

//#define raGET_CARD_INDEX(crd) ((crd->GetSuit() << 3) + gmUtil::m_value_trans[crd->GetValue()])

// Calculates the total number of points in a hand
#define gmTotalPoints(X) ( \
gmUtil::m_total_points[(gmUtil::m_suit_mask[0] & X) >> gmUtil::m_suit_rs[0]] + \
gmUtil::m_total_points[(gmUtil::m_suit_mask[1] & X) >> gmUtil::m_suit_rs[1]] + \
gmUtil::m_total_points[(gmUtil::m_suit_mask[2] & X) >> gmUtil::m_suit_rs[2]] + \
gmUtil::m_total_points[(gmUtil::m_suit_mask[3] & X) >> gmUtil::m_suit_rs[3]]  \
)

#endif

