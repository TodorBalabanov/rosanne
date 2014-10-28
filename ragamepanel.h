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

#ifndef _RAGAME_H
#define _RAGAME_H

#include "racommon.h"
#include "gmengine.h"
#include "rainfo.h"
#include "raevents.h"
#include "rabid.h"
#include "raplayer.h"
#include "raconfig.h"
#include "ggpanel.h"
#include "ggcard.h"

//#define raGAME_PLAY_TILL_END 0

#define raTOTAL_CARD_BACKS 2
#define raMAX_CARDS_PER_HAND 8
#define raCARD_VERT_RELIEF (12)
#define raCARD_HORZ_RELIEF (GG_CARD_WIDTH / 4)
#define raCARD_PANEL_RELIEF 20

#define raGAME_CARD_BACK_SEL 0

#define raGAME_ARROW_RELIEF 8

#define raGAME_FOUR_JACKS (0x80808080)
#define raGAME_ALL_LOW_CARDS (0x0F0F0F0F)
#define raGAME_ALL_HIGH_CARDS (0xF0F0F0F0)


// For testing purposes
#define raTEST_DATA_FILE wxT("ra_test_data.ini")
#define raTEXT_SEED wxT("rand/seed")
#define raTEXT_IDX wxT("rand/idx")
#define raTEXT_DEALER wxT("deal/dealer")
#define raTEXT_DEAL_ROUND wxT("deal_round")



enum {
	raHAND_VERTICAL = 0,
	raHAND_HORIZONTAL
};

enum {
	raGAME_ORIENT_ALL_HORZ = 0,
	raGAME_ORIENT_MIXED
};

// TODO : If raHandCard is no longer used, remove
typedef struct tagRA_HAND_CARD {
	//int suit;
	//int value;
	int index;
	int x;
	int y;
	int width;
	int height;
} raHandCard, *praHandCard;

typedef struct tagRA_HAND {
	unsigned long cards;
	int count;
	int card_indexes[raMAX_CARDS_PER_HAND];
} raHand, *praHand;

typedef struct tagRA_BACK_DRAW_INFO {
	bool draw_bid;
	int bid_loc;
	int bid;
} raBackDrawInfo;

class raGamePanel: public ggPanel {
private:
	wxBitmap *m_tile;
	DECLARE_EVENT_TABLE()

	// Disallow copy constructor/assignment operators
	raGamePanel(const raGamePanel &);
	raGamePanel & operator=(const raGamePanel &);

	wxBitmap *m_card_faces[gmTOTAL_CARDS];
	wxBitmap *m_card_backs[raTOTAL_CARD_BACKS];
	gmEngine m_engine;
	raInfo *m_info;
	raBid *m_bid;

	// Related to the display of hands
	raHand m_hands[gmTOTAL_PLAYERS];
	wxRect m_hand_rects[gmTOTAL_PLAYERS];
	wxRect m_hand_card_rects[gmTOTAL_PLAYERS][raMAX_CARDS_PER_HAND];
	int m_hand_rot;

	// Related to the display of cards played in a trick
	//int m_trick_cards[gmTOTAL_PLAYERS];
	gmTrick m_trick;
	wxRect m_trick_card_rects[gmTOTAL_PLAYERS];
	bool m_wait_trick;

	// To check whether the deal is still being played
	bool m_deal_ended;

	// Players
	raPlayer m_players[gmTOTAL_PLAYERS];

	// Game points and penalties
	int m_game_pts[gmTOTAL_TEAMS];
	int m_pnlties[gmTOTAL_PLAYERS];

	// Direction of play
	bool m_clockwise;

	// Font used to write text
	wxFont m_font_bold;

	// Hand orientation
	int m_orientation;

	// Prefereed card back
	int m_pref_card_back;

	// Play card on single click
	int m_play_card_on;

	// Auto play single choice
	bool m_auto_play;

	// Show bid bubbles
	bool m_show_bidbubbles;

	// Saved rules
	gmRules m_saved_rules;

	// Saved auction history
	wxString m_bid_history;

	// Bitmaps to be loaded from xpms

	wxBitmap *m_bmp_green_arrow_bottom;
	wxBitmap *m_bmp_green_arrow_top;
	wxBitmap *m_bmp_green_arrow_right;
	wxBitmap *m_bmp_green_arrow_left;

	wxBitmap *m_bmp_red_arrow_bottom;
	wxBitmap *m_bmp_red_arrow_top;
	wxBitmap *m_bmp_red_arrow_right;
	wxBitmap *m_bmp_red_arrow_left;

	wxBitmap *m_bmp_bubble_arrow_bottom;
	wxBitmap *m_bmp_bubble_arrow_left;
	wxBitmap *m_bmp_bubble_arrow_right;
	wxBitmap *m_bmp_bubble_arrow_top;

	wxBitmap *m_bmp_bubble_corner_ne;
	wxBitmap *m_bmp_bubble_corner_nw;
	wxBitmap *m_bmp_bubble_corner_se;
	wxBitmap *m_bmp_bubble_corner_sw;

	wxBitmap *m_bmp_bubble_edge_bottom;
	wxBitmap *m_bmp_bubble_edge_left;
	wxBitmap *m_bmp_bubble_edge_right;
	wxBitmap *m_bmp_bubble_edge_top;

	void OnSize(wxSizeEvent& event);
	bool RedrawBack(raBackDrawInfo *info = NULL);
	bool DrawHand(int loc, int x, int y, int orientation = raHAND_HORIZONTAL);
	bool DrawTrick();
	bool DrawTrump();
	void OnInfo(raInfoEvent& event);
	void OnBid(raBidEvent& event);
	void OnLeftDClick(wxMouseEvent &event);
	void OnLeftUp(wxMouseEvent &event);
	int GetCardAtPos(wxPoint pt, int loc = gmPLAYER_INVALID);
	int GetHandAtPos(wxPoint pt);
	bool Continue();
	bool ResetDeal();
	bool ResetGame();
	bool UpdateHands(unsigned long *hands);
	int PlayCard(int card, int loc = gmPLAYER_INVALID);
	int SetTrump(int card, int loc = gmPLAYER_INVALID);
	int ShowTrump(int loc = gmPLAYER_INVALID);
	int MakeBid(int bid, int loc = gmPLAYER_INVALID);
	bool UpdateDrawAndRefresh(bool udpate = true, raBackDrawInfo *info = NULL);
	//bool UpdateTrick(gmTrick *trick);
	bool HideInfo(gmEngineData *data, int player);
	bool HasDealEnded(int *winner = NULL);
	bool EndDeal(bool abandon = false);
	bool BeginBusyState();
	bool EndBusyState();
	int CheckOppTrumps();
	bool OnCardClick(wxPoint pt);
public:
	raGamePanel(const wxWindow* parent);
	virtual ~raGamePanel();
	bool SetTile(wxBitmap *tile);
	bool SetInfoPanel(raInfo *info_panel);
	bool NewGame(int dealer = gmPLAYER_INVALID, bool immediate = true);
	bool NewDeal();
	bool SetClockwise(bool flag);
	bool GetClockwise();
	bool ReloadFromConfig();
	bool ShowAuction();
	bool ShowLastTrick();
};
#endif
