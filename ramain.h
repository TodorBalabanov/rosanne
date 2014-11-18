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

#ifndef _RAMAIN_H_
#define _RAMAIN_H_

#include "racommon.h"
#include "ragamepanel.h"
#include "rainfo.h"
#include "raupdate.h"
#include "raconfig.h"
#include "radlgabout.h"
#include "radlgprefs.h"
#include "radlgrules.h"

#include "raApp.h"

enum {
	raID_NEW_GAME = 10000,
	raID_EXIT,
	raID_PREFERENCES,
	raID_RULES,
	raID_BID_HISTORY,
	raID_LAST_TRICK,
	raID_HELP,
	raID_ABOUT
};

#define raSBAR_FIELDS 2

#define raLOG_DIR wxT("log")
#define raLOG_FILE_PREFIX wxT("rosanne")
#define raLOG_FILE_EXTN wxT("log")
#define raLOG_FILE_DELIM wxT("_")

#define raGUI_XRS wxT("gui.xrs")

class raFrame;

// Declare our main frame class
class raFrame : public wxFrame {
public:
	// Constructor
	raFrame(const wxString& title);

	// Event handlers
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnGameNew(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnPreferences(wxCommandEvent& event);
	void OnRules(wxCommandEvent& event);
	void OnAuction(wxCommandEvent& event);
	void OnLastTrick(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnUpdate(raUpdateEvent& event);

private:
	wxSplitterWindow *m_split_main;
	raGamePanel *m_game;
	raInfo *m_info;
	bool ShowPreferences();
	bool ShowRules();

	// This class handles events
	DECLARE_EVENT_TABLE()
	// Disallow copy constructor/assignment operators
	raFrame(const raFrame &);
	raFrame & operator=(const raFrame &);
};

// Implements raApp& GetApp()
DECLARE_APP(raApp)
#endif
