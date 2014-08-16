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

#ifndef _RADLGRULES_H_
#define _RADLGRULES_H_

//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma interface "radlgrules.h"
//#endif

#include "wx/xrc/xmlres.h"

class raDlgRules: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( raDlgRules )
    DECLARE_EVENT_TABLE()
private:
	// Disallow copy constructor/assignment operators
	raDlgRules(const raDlgRules &);
    raDlgRules & operator=(const raDlgRules &);
public:
    raDlgRules( );
    raDlgRules( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style);
    void OnInitDialog( wxInitDialogEvent& event );
    void OnRulesBtnApplyClick( wxCommandEvent& event );
};

#endif
