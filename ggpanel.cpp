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

#include "ggpanel.h"

// TODO : Reduce flickering
// TODO : Change log statements to include wxT(__FILE__) and __LINE__

BEGIN_EVENT_TABLE(ggPanel, wxPanel)
	EVT_PAINT(ggPanel::OnPaint)
	//EVT_ERASE_BACKGROUND(wxcTable::OnErase)
END_EVENT_TABLE()

ggPanel::ggPanel(const wxWindow* parent): wxPanel((wxWindow*)parent)
{
	m_work = NULL;
	m_back = NULL;
	m_f_invalid = true;
	Size();
}
ggPanel::~ggPanel()
{
	// Delete the m_work and m_back bitmaps
	if(m_back)
		delete m_back;
	if(m_work)
		delete m_work;
}
bool ggPanel::Size()
{
	int x, y;

	GetClientSize(&x, &y);

	if(m_work)
		delete m_work;

	if(m_back)
		delete m_back;

	m_work = new wxBitmap(x, y, -1);
	m_back = new wxBitmap(x, y, -1);

	if(!m_back)
	{
		wxLogError(wxT("ggPanel::Size - Creation of m_back failed in wxcTable::Resize"));
		return false;
	}
	if(!m_work)
	{
		wxLogError(wxT("ggPanel::Size - Creation of m_work failed in wxcTable::Resize"));
		return false;
	}

	////wxLogDebug("wxcTable::Size - Exiting");

	return true;
}
void ggPanel::OnPaint(wxPaintEvent &event)
{
	Paint();
}
bool ggPanel::Paint()
{
	////wxLogDebug("wxcTable::Paint - Entering");
	wxPaintDC pdc(this);
	wxMemoryDC mdc;

	if(!pdc.Ok())
	{
		wxLogError(wxT("wxcTable::Paint - Paint DC is not OK"));
		return false;
	}

	/*if(!mdc.Ok())
	{
		wxLogError("wxcTable::Paint - Memory DC is not OK");
		return false;
	}*/

	if(m_back)
	{
		mdc.SelectObject(*m_work);
		if(!pdc.Blit(wxPoint(0, 0), wxSize(m_work->GetWidth(), m_work->GetHeight()), &mdc, wxPoint(0, 0)))
		{
			wxLogError(wxT("wxcTable::Paint - Blt to wxPaintDC failed in wxCardTable::Paint"));
			return false;
		}
		mdc.SelectObject(wxNullBitmap);
	}
	return false;

	////wxLogDebug("wxcTable::Paint - Exiting");

}
bool ggPanel::RefreshScreen()
{
	wxClientDC tdc(this);
	wxMemoryDC wdc;
	wxPoint pos;
	wxSize size;

	////wxLogDebug("wxcTable::RefreshTable - Entering");

	if(m_f_invalid)
	{
		wdc.SelectObject(*m_work);

		pos = m_rect_invalid.GetPosition();
		size = m_rect_invalid.GetSize();

		//wxLogDebug(wxString::Format("Blit area %d-%d %d-%d", pos.x, pos.y, size.GetWidth(), size.GetHeight()));
		if(!tdc.Blit(pos, size, &wdc, pos))
		{
			wxLogError(wxT("wxcTable::RefreshTable - Blit to wxcTable client failed"));
			return false;
		}
		wdc.SelectObject(wxNullBitmap);

		m_f_invalid = false;
		m_rect_invalid = wxRect(0, 0, 0, 0);
	}

	////wxLogDebug("wxcTable::RefreshTable - Exiting");

	return true;
}
bool ggPanel::BlitToBack(wxCoord xdest, wxCoord ydest,
	wxCoord width, wxCoord height, wxDC* source, wxCoord xsrc,
	wxCoord ysrc, int logicalFunc, bool useMask,
	wxCoord xsrcMask, wxCoord ysrcMask)
{
	int x, y;
	wxMemoryDC bdc, wdc;

	bdc.SelectObject(*m_back);
	wdc.SelectObject(*m_work);

	if(!bdc.Ok())
		return false;

	if(!wdc.Ok())
		return false;

	if(!bdc.Blit(xdest, ydest, width, height, source, xsrc, ysrc, logicalFunc, useMask, xsrcMask, ysrcMask))
		return false;

	GetClientSize(&x, &y);
	//if(!wdc.Blit(0, 0, x, y, &bdc, 0, 0))
	//	return false;
	if(!wdc.Blit(xdest, ydest, width, height, &bdc, xdest, ydest))
		return false;

	m_rect_diff = wxRect(0, 0, 0, 0);
	m_rect_invalid = wxRect(0, 0, x, y);
	m_f_invalid = true;

	return true;

}
bool ggPanel::BlitToFront(wxCoord xdest, wxCoord ydest,
	wxCoord width, wxCoord height, wxDC* source, wxCoord xsrc,
	wxCoord ysrc, int logicalFunc, bool useMask,
	wxCoord xsrcMask, wxCoord ysrcMask)
{
	wxMemoryDC wdc;
	wdc.SelectObject(*m_work);

	if(!wdc.Ok())
		return false;

	if(!wdc.Blit(xdest, ydest, width, height, source, xsrc, ysrc, logicalFunc, useMask, xsrcMask, ysrcMask))
		return false;

	// Add the area drawn to m_rect_diff
	m_rect_diff.Union(wxRect(xdest, ydest, width, height));
	// Add the area drawn to m_rect_invalid
	m_rect_invalid.Union(wxRect(xdest, ydest, width, height));
	m_f_invalid = true;

	return true;
}

bool ggPanel::DrawTextOnBack(wxString text, wxPoint pt, wxColour colour, wxFont font)
{
	int x, y;
	wxMemoryDC bdc, wdc;

	bdc.SelectObject(*m_back);
	wdc.SelectObject(*m_work);

	if(!bdc.Ok())
		return false;

	if(!wdc.Ok())
		return false;

	// Set the text foreground colour and the font
	if(colour != wxNullColour)
	{
		bdc.SetTextForeground(colour);
		wdc.SetTextForeground(colour);
	}
	if(font != wxNullFont)
	{
		bdc.SetFont(font);
		wdc.SetFont(font);
	}

	bdc.DrawText(text, pt);
	wdc.DrawText(text, pt);

	GetClientSize(&x, &y);
	m_rect_diff = wxRect(0, 0, 0, 0);
	m_rect_invalid = wxRect(0, 0, x, y);
	m_f_invalid = true;

	return true;
}

bool ggPanel::ClearDifference()
{
	wxMemoryDC bdc, wdc;
	bdc.SelectObject(*m_back);
	wdc.SelectObject(*m_work);

	if(!bdc.Ok())
		return false;

	if(!wdc.Ok())
		return false;

	if(!wdc.Blit(m_rect_diff.x, m_rect_diff.y, m_rect_diff.GetWidth(), m_rect_diff.GetHeight(),
		&bdc, m_rect_diff.x, m_rect_diff.y))
		return false;

	m_rect_invalid.Union(m_rect_diff);
	m_rect_diff = wxRect(0, 0, 0, 0);
	m_f_invalid = true;

	return true;
}
