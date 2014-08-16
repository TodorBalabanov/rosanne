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

#include "rainfo.h"
#include "ragamepanel.h"

BEGIN_EVENT_TABLE(raInfo, wxPanel)
	EVT_BUTTON(XRCID("m_rainfo_btn"), raInfo::OnButtonClick)
END_EVENT_TABLE()

raInfo::raInfo(wxWindow* parent)//: wxPanel((wxWindow*)parent)
{
	m_game = NULL;
	m_curr_cmd = raINFO_CMD_NONE;

	ResetDetails();

	if (!wxXmlResource::Get()->LoadPanel(this, parent, wxT("raInfo")))
		wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));

	m_button = XRCCTRL(*this, "m_rainfo_btn", wxButton);
	m_dealno = XRCCTRL(*this, "m_rainfo_dealno", wxStaticText);
	m_dealer = XRCCTRL(*this, "m_rainfo_dealer", wxStaticText);
	m_bid = XRCCTRL(*this, "m_rainfo_bid", wxStaticText);
	m_trump = XRCCTRL(*this, "m_rainfo_trump", wxStaticText);

	m_nspts = XRCCTRL(*this, "m_rainfo_nspts", wxStaticText);
	m_ewpts = XRCCTRL(*this, "m_rainfo_ewpts", wxStaticText);


	m_spnlty = XRCCTRL(*this, "m_rainfo_spnlty", wxStaticText);
	m_npnlty = XRCCTRL(*this, "m_rainfo_npnlty", wxStaticText);
	m_epnlty = XRCCTRL(*this, "m_rainfo_epnlty", wxStaticText);
	m_wpnlty = XRCCTRL(*this, "m_rainfo_wpnlty", wxStaticText);

	m_instr = XRCCTRL(*this, "m_rainfo_instr", wxStaticText);

	m_button->Show(false);


	SetDetails(&m_details);

	return;

}
raInfo::~raInfo()
{
	return;
}

bool raInfo::SetDetails(raInfoDetails *details)
{

	// Validate input data and set the deal number
	if(details->deal_no < 0)
	{
		wxLogError(wxString::Format(wxT("Negative deal number passed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	m_dealno->SetLabel(wxString::Format(wxT("%d"), details->deal_no));

	// Validate input data and set the dealer
	if((details->dealer < gmPLAYER_INVALID) || (details->dealer > gmTOTAL_PLAYERS))
	{
		wxLogError(wxString::Format(wxT("Incorrect dealer. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(details->dealer == gmPLAYER_INVALID)
		m_dealer->SetLabel(wxT("N/A"));
	else
		m_dealer->SetLabel((wxString::Format(wxT("%s"), gmUtil::m_long_locs[details->dealer].c_str())));


	// Validate input data and set the bidder
	if((details->bidder < gmPLAYER_INVALID) || (details->bidder > gmTOTAL_PLAYERS))
	{
		wxLogError(wxString::Format(wxT("Incorrect bidder. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(details->bidder == gmPLAYER_INVALID)
		m_bid->SetLabel(wxT("N/A"));
	else if (details->bid == gmBID_ALL)
		m_bid->SetLabel(wxString::Format(wxT("All by %s"), gmUtil::m_long_locs[details->bidder].c_str()));
	else
		m_bid->SetLabel(wxString::Format(wxT("%d by %s"), details->bid, gmUtil::m_long_locs[details->bidder].c_str()));

	// Validate input data and set the trump
	if((details->trump < gmSUIT_INVALID) || (details->trump > gmTOTAL_SUITS))
	{
		wxLogError(wxString::Format(wxT("Incorrect trump suit. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(details->trump == gmSUIT_INVALID)
		m_trump->SetLabel(wxString::Format(wxT("Not Shown")));
	else
		m_trump->SetLabel(wxString::Format(wxT("%s"), gmUtil::m_suits[details->trump].c_str()));

	m_nspts->SetLabel(wxString::Format(wxT("%d"), details->points[0]));
	m_ewpts->SetLabel(wxString::Format(wxT("%d"), details->points[1]));

	m_spnlty->SetLabel(wxString::Format(wxT("%s-%d"),
		gmUtil::m_short_locs[0].c_str(), details->pnlties[0]));
	m_wpnlty->SetLabel(wxString::Format(wxT("%s-%d"),
		gmUtil::m_short_locs[1].c_str(), details->pnlties[1]));
	m_npnlty->SetLabel(wxString::Format(wxT("%s-%d"),
		gmUtil::m_short_locs[2].c_str(), details->pnlties[2]));
	m_epnlty->SetLabel(wxString::Format(wxT("%s-%d"),
		gmUtil::m_short_locs[3].c_str(), details->pnlties[3]));

	memcpy(&m_details, details, sizeof(raInfoDetails));
	return true;
}
void raInfo::GetDetails(raInfoDetails *details)
{
	memcpy(details, &m_details, sizeof(raInfoDetails));
}

bool raInfo::SetInstruction(wxString instruction, int cmd)
{
	if((cmd != m_curr_cmd) || (cmd == raINFO_CMD_NONE))
	{
		m_instruction = instruction;

		// Depending on the command id passed,
		// enable/disable the command button
		// and set it's text accordingly
		switch(cmd)
		{
		case raINFO_CMD_NONE:
			m_button->Show(false);
			break;
		case raINFO_CMD_NEW_DEAL:
			m_button->Show(true);
			m_button->SetLabel(raINFO_DEAL_TEXT);
			if(m_instruction.IsEmpty())
				m_instruction = wxT("Click on the button below to start a new Deal.");
			break;
		case raINFO_CMD_SHOW_TRUMP:
			m_button->Show(true);
			m_button->SetLabel(raINFO_SHOW_TRUMP_TEXT);
			break;
		default:
			wxLogError(wxString::Format(wxT("Unexpected value in switch statement. %s:%d"), wxT(__FILE__), __LINE__));
			return false;
		}
		m_curr_cmd = cmd;
	}

	// Set the instuction text, wrap and fit

	m_instr->SetLabel(m_instruction);
	//TODO : Remove hardcoding of 10
	m_instr->Wrap(this->GetClientSize().GetWidth() - (2 * 10));
	m_instr->Update();
	m_instr->Refresh();


	return true;
}
bool raInfo::SetGamePanel(raGamePanel *game_panel)
{
	wxASSERT(game_panel);
	m_game = game_panel;
	return true;
}

bool raInfo::ResetDetails(bool refresh)
{
	int i;
	m_details.bid = 0;
	m_details.bidder = gmPLAYER_INVALID;
	m_details.dealer = gmPLAYER_INVALID;
	m_details.deal_no = 1;
	for(i = 0; i < gmTOTAL_TEAMS; i++)
	{
		m_details.points[i] = 5;
	}
	for(i = 0; i < gmTOTAL_PLAYERS; i++)
	{
		m_details.pnlties[i] = 0;
	}
	m_details.trump = gmSUIT_INVALID;
	if(refresh)
	{
		SetDetails(&m_details);
	}
	return true;
}

//
// private methods
//

void raInfo::OnButtonClick(wxCommandEvent &event)
{
	wxASSERT(m_game);
	wxASSERT(m_curr_cmd != raINFO_CMD_NONE);

	raInfoEvent new_event;
	new_event.SetCommand(m_curr_cmd);
	m_game->AddPendingEvent(new_event);
}
