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

#ifndef _RABID_H
#define _RABID_H

#include "racommon.h"

#define raBID_BTN_ROWS 5
#define raBID_BTN_COLS 3
#define raBID_TOTAL_BTNS 15

#define raBID_BTN_ID_START 100
#define raBID_BTN_ID_MAX (raBID_BTN_ID_START + raBID_TOTAL_BTNS - 1)
#define raBID_BTN_ID_ALL 150
#define raBID_BTN_ID_PASS 151

#define raBID_MIN_BTN_WIDTH 10

#define raGetBidFromId(X) (X - raBID_BTN_ID_START + 14)

#define raBID_PNL_RELIEF 2

class raGamePanel;

class raBid: public wxPanel {
private:
	DECLARE_EVENT_TABLE()

	// Disallow copy constructor/assignment operators
	raBid(const raBid &);
	raBid & operator=(const raBid &);

	int m_min_bid;

	wxPanel *m_main_panel;
	wxGridSizer *m_main_sizer;
	wxBoxSizer *m_main_panel_sizer;
	wxPanel *m_head_panel;
	wxGridSizer *m_head_panel_sizer;
	wxStaticText *m_head_panel_text;
	wxFont m_bold_font;
	wxPanel *m_bidbtn_panel;
	wxGridSizer *m_bidbtn_panel_sizer;
	wxButton *m_buttons[raBID_TOTAL_BTNS];

	wxPanel *m_btns_panel;
	wxGridSizer *m_btns_panel_sizer;
	wxButton *m_button_all;
	wxButton *m_button_pass;

	raGamePanel *m_game;

	void OnButtonClick(wxCommandEvent &event);
public:
	raBid(const wxWindow* parent);
	~raBid();
	bool SetGamePanel(raGamePanel *game_panel);
	bool SetPassable(bool passable = true);
	bool SetMinimumBid(int min_bid);
};

#endif
