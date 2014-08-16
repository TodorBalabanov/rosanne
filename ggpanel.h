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

#ifndef _GGPANEL_H_
#define _GGPANEL_H_

#include "wx/wx.h"

class ggPanel: public wxPanel
{
private:
	wxBitmap *m_back, *m_work;
	bool m_f_invalid;
	wxRect m_rect_diff, m_rect_invalid;
	DECLARE_EVENT_TABLE()
	// Disallow copy constructor/assignment operators
	ggPanel(const ggPanel &);
    ggPanel & operator=(const ggPanel &);
public:
	ggPanel(const wxWindow* parent);
	~ggPanel();
	bool Size();
	bool Paint();
	virtual void OnPaint(wxPaintEvent &event);
	bool RefreshScreen();
	bool BlitToBack(wxCoord xdest, wxCoord ydest,
		wxCoord width, wxCoord height, wxDC* source, wxCoord xsrc,
		wxCoord ysrc, int logicalFunc = wxCOPY, bool useMask = false,
		wxCoord xsrcMask = -1, wxCoord ysrcMask = -1);
	bool BlitToFront(wxCoord xdest, wxCoord ydest,
		wxCoord width, wxCoord height, wxDC* source, wxCoord xsrc,
		wxCoord ysrc, int logicalFunc = wxCOPY, bool useMask = false,
		wxCoord xsrcMask = -1, wxCoord ysrcMask = -1);
	bool DrawTextOnBack(wxString text, wxPoint pt, wxColour colour = wxNullColour, wxFont font = wxNullFont);
	bool ClearDifference();
	//bool DrawBack();
};

#endif
