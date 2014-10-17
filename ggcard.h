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

#ifndef _GGCARD_H_
#define _GGCARD_H_

#include "wx/wx.h"
#include "wx/filesys.h"
#include "wx/fs_zip.h"
#include "wx/xrc/xmlres.h"
#include "wx/image.h"
//#include "wx/thread.h"
#define GG_CARD_CARD_COUNT 52

#define GG_CARD_SPADES 3
#define GG_CARD_HEARTS 2
#define GG_CARD_DIAMONDS 1
#define GG_CARD_CLUBS 0

#define GG_CARD_ACE 0
#define GG_CARD_TWO 1
#define GG_CARD_THREE 2
#define GG_CARD_FOUR 3
#define GG_CARD_FIVE 4
#define GG_CARD_SIX 5
#define GG_CARD_SEVEN 6
#define GG_CARD_EIGHT 7
#define GG_CARD_NINE 8
#define GG_CARD_TEN 9
#define GG_CARD_JACK 10
#define GG_CARD_QUEEN 11
#define GG_CARD_KING 12

#define GG_CARD_BACK_1 20
#define GG_CARD_BACK_2 21
#define GG_CARD_JOKER_1 30
#define GG_CARD_JOKER_2 31


#define GG_CARD_TOTAL_SUITS 4
#define GG_CARD_TOTAL_VALUES 13

#define GG_CARD_WIDTH 71
#define GG_CARD_HEIGHT 96

#define GG_CARD_XRS wxT("cards.xrs")

class ggCard:public wxObject {
public:
	ggCard();
	ggCard(int suit, int value);
	ggCard(int other);
	~ggCard();
	wxBitmap * m_face;
	bool BlitTo(wxDC* dest, wxCoord xdest, wxCoord ydest, int logicalFunc = wxCOPY);
	wxBitmap * GetFace();
	void SelectToDC(wxMemoryDC *mdc);
	// Disallow copy constructor/assignment operators
	ggCard(const ggCard &);
	ggCard & operator=(const ggCard &);
private:
	static bool s_init;
	static wxMutex s_mutex;
	static wxBitmap s_mask_bmp;
	bool LoadFace(wxString res_name);
};

#endif
