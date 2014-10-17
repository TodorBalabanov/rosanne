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

#ifndef _RADLGPREFS_H_
#define _RADLGPREFS_H_

//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma interface "radlgprefs.h"
//#endif

#include "wx/xrc/xmlres.h"

#define raPREFS_PLAYCARDON_SCLICK 0
#define raPREFS_PLAYCARDON_DCLICK 1
#define raPREFS_CARDBACK_BLUE 0
#define raPREFS_CARDBACK_RED 1

class raDlgPrefs: public wxDialog {
	DECLARE_DYNAMIC_CLASS( raDlgPrefs )
	DECLARE_EVENT_TABLE()
private:
	// Disallow copy constructor/assignment operators
	raDlgPrefs(const raDlgPrefs &);
	raDlgPrefs & operator=(const raDlgPrefs &);

public:
	raDlgPrefs( );
	raDlgPrefs( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style);
	void OnInitDialog( wxInitDialogEvent& event );
	void OnPrefsBtnApplyClick( wxCommandEvent& event );

};

#endif
