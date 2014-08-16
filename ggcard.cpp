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

#include "ggcard.h"

bool ggCard::s_init = false;
wxMutex ggCard::s_mutex;
wxBitmap ggCard::s_mask_bmp;

//
// Constructor
//

ggCard::ggCard()
{
	m_face = NULL;

	// If the constructor is being called for the first time
	// 1. Load the resources used by the library.
	// 2. Load the mask image

	if(!s_init)
	{
		wxMutexLocker lock(s_mutex);
		if(!s_init)
		{
			wxFileSystem::AddHandler(new wxZipFSHandler);
			wxImage::AddHandler(new wxXPMHandler);

			wxXmlResource::Get()->InitAllHandlers();
			if(!wxXmlResource::Get()->Load(GG_CARD_XRS))
			{
				wxLogError(wxString::Format(wxT("Failed to load xrs %s. %s:%d"),GG_CARD_XRS,  wxT(__FILE__), __LINE__));
				return;
			}
			s_mask_bmp = wxXmlResource::Get()->LoadBitmap(wxT("mask"));
			if(!s_mask_bmp.Ok())
			{
				wxLogError(wxString::Format(wxT("Failed to load mask bitmap. %s:%d"), wxT(__FILE__), __LINE__));
				return;
			}
			s_init = true;
		}
	}
}

ggCard::ggCard(int suit, int value)
{
	wxASSERT((suit >= 0 ) && (suit < GG_CARD_TOTAL_SUITS));
	wxASSERT((value >= 0) && (suit < GG_CARD_TOTAL_VALUES));

	ggCard();
	if(!LoadFace(wxString::Format(wxT("face_%02d"), (suit * GG_CARD_TOTAL_VALUES) + value)))
		wxLogError(wxString::Format(wxT("LoadFace failed. %s:%d"), wxT(__FILE__), __LINE__));

	wxASSERT(m_face);
	return;
}

ggCard::ggCard(int other)
{
	ggCard();
	switch(other)
	{
	case GG_CARD_BACK_1:
		if(!LoadFace(wxT("back_00")))
			wxLogError(wxString::Format(wxT("LoadFace failed. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	case GG_CARD_BACK_2:
		if(!LoadFace(wxT("back_01")))
			wxLogError(wxString::Format(wxT("LoadFace failed. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	case GG_CARD_JOKER_1:
		if(!LoadFace(wxT("joker_01")))
			wxLogError(wxString::Format(wxT("LoadFace failed. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	case GG_CARD_JOKER_2:
		if(!LoadFace(wxT("joker_01")))
			wxLogError(wxString::Format(wxT("LoadFace failed. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	default:
		wxLogError(wxString::Format(wxT("Incorrect argument passed. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	}

	wxASSERT(m_face);
}

//
// Destructor
//

ggCard::~ggCard()
{
	if(m_face)
	{
		delete m_face;
		m_face = NULL;
	}
}

//
// Public methods
//

bool ggCard::BlitTo(wxDC* dest, wxCoord xdest, wxCoord ydest, int logicalFunc)
{
	wxMemoryDC mdc;

	wxASSERT(dest);

	mdc.SelectObject(*m_face);
	if(!dest->Blit(xdest, ydest, GG_CARD_WIDTH, GG_CARD_HEIGHT, &mdc, 0, 0, logicalFunc, true))
	{
		wxLogError(wxString::Format(wxT("Blit failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	return true;
}
wxBitmap * ggCard::GetFace()
{
	wxASSERT(m_face);
	return m_face;
}
void ggCard::SelectToDC(wxMemoryDC *mdc)
{
	wxASSERT(mdc);
	mdc->SelectObject(*m_face);
}

//
// Private methods
//

bool ggCard::LoadFace(wxString res_name)
{
	wxBitmap bmp_temp1, bmp_temp2;
	wxImage img_mask, img_face;
	wxMemoryDC mdc1, mdc2;
	wxColour col_mask(*wxWHITE);

	wxASSERT(!res_name.IsEmpty());

	// Load the bitmap from the resource file
	bmp_temp1 = wxXmlResource::Get()->LoadBitmap(res_name);
	if(!bmp_temp1.Ok())
	{
		wxLogError(wxString::Format(wxT("Failed to load resource %s. %s:%d"), res_name.c_str(), wxT(__FILE__), __LINE__));
		return false;
	}

	// The widths of mask and the xpm image are differt.
	// Hence create a new bitmap with correct dimensions and
	// copy the data to the same.
	if(!bmp_temp2.Create(GG_CARD_WIDTH, GG_CARD_HEIGHT, -1))
	{
		wxLogError(wxString::Format(wxT("Failed to create bitmap. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	mdc1.SelectObject(bmp_temp1);
	mdc2.SelectObject(bmp_temp2);

	if(!mdc2.Blit(0, 0, GG_CARD_WIDTH, GG_CARD_HEIGHT, &mdc1, 0, 0))
	{
		wxLogError(wxString::Format(wxT("Blit failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	// Create images of face and mask and set the mask for the face
	img_face = bmp_temp2.ConvertToImage();
	img_mask = s_mask_bmp.ConvertToImage();

	if(!img_face.SetMaskFromImage(img_mask, col_mask.Red(), col_mask.Green(), col_mask.Blue()))
		wxLogError(wxString::Format(wxT("Failed to set mask from image. %s:%d"), wxT(__FILE__), __LINE__));

	// And then finally create a bitmap from the masked image
	m_face = new wxBitmap(img_face, -1);

	if(!m_face)
	{
		wxLogError(wxString::Format(wxT("Creation of bitmap from image failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	return true;
}
