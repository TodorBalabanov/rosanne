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

#ifndef _RAINFO_H
#define _RAINFO_H

#include "racommon.h"
#include "gmutil.h"

#include "raInfoDetails.h"

#ifdef raREAD_SEED_FROM_FILE
#include "wx/wfstream.h"
#include "wx/fileconf.h"
#endif

enum {
	raINFO_CMD_NONE = 1,
	raINFO_CMD_NEW_DEAL,
	raINFO_CMD_SHOW_TRUMP
};

#define raINFO_SHOW_TRUMP_TEXT wxT("Show Trump")
#define raINFO_DEAL_TEXT wxT("New Deal")

class raGamePanel;

class raInfo: public wxPanel {
private:
	DECLARE_EVENT_TABLE()

	// Disallow copy constructor/assignment operators
	raInfo(const raInfo &);
	raInfo & operator=(const raInfo &);

	wxButton *m_button;
	wxStaticText *m_dealno;
	wxStaticText *m_dealer;
	wxStaticText *m_bid;
	wxStaticText *m_trump;

	wxStaticText *m_nspts;
	wxStaticText *m_ewpts;

	wxStaticText *m_spnlty;
	wxStaticText *m_npnlty;
	wxStaticText *m_epnlty;
	wxStaticText *m_wpnlty;

	wxStaticText *m_instr;

	int m_curr_cmd;
	raGamePanel *m_game;
	raInfoDetails m_details;
	wxString m_instruction;

	void OnButtonClick(wxCommandEvent &event);
public:
	raInfo(wxWindow* parent);
	virtual ~raInfo();
	bool SetDetails(raInfoDetails *details);
	void GetDetails(raInfoDetails *details);
	bool SetInstruction(wxString instruction, int cmd = raINFO_CMD_NONE);
	bool SetGamePanel(raGamePanel *game_panel);
	bool ResetDetails(bool refresh = false);
};

#endif
