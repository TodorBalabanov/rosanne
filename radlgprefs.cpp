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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "radlgprefs.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "radlgprefs.h"
#include "raconfig.h"


IMPLEMENT_DYNAMIC_CLASS( raDlgPrefs, wxDialog )

BEGIN_EVENT_TABLE( raDlgPrefs, wxDialog )
	EVT_INIT_DIALOG( raDlgPrefs::OnInitDialog )
	EVT_BUTTON( XRCID("m_radlgprefs_apply"), raDlgPrefs::OnPrefsBtnApplyClick )
END_EVENT_TABLE()

raDlgPrefs::raDlgPrefs( ) {
}

raDlgPrefs::raDlgPrefs( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) {
	SetParent(parent);

	if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("raDlgPrefs")))
		wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));

	if (GetSizer()) {
		GetSizer()->SetSizeHints(this);
	}
}

void raDlgPrefs::OnInitDialog( wxInitDialogEvent& event ) {
	wxComboBox *combo_playcardon;
	wxComboBox *combo_cardback;
	wxCheckBox *check_autoplay;
	wxCheckBox *check_bidbubbles;
	raConfData conf_data;

	raConfig::GetInstance()->GetData(&conf_data);
	combo_playcardon = XRCCTRL(*this, "m_radlgprefs_playcardon", wxComboBox);
	switch(conf_data.prefs_data.play_card_on) {
	case raCONFIG_PREFS_PLAYCARDON_SCLICK:
		combo_playcardon->SetSelection(raPREFS_PLAYCARDON_SCLICK);
		break;
	case raCONFIG_PREFS_PLAYCARDON_DCLICK:
		combo_playcardon->SetSelection(raPREFS_PLAYCARDON_DCLICK);
		break;
	default:
		wxLogError(wxString::Format(wxT("Unexpected value. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	}

	combo_cardback = XRCCTRL(*this, "m_radlgprefs_cardback", wxComboBox);

	switch(conf_data.prefs_data.card_back) {
	case raCONFIG_PREFS_CARDBACK_BLUE:
		combo_cardback->SetSelection(raPREFS_CARDBACK_BLUE);
		break;
	case raCONFIG_PREFS_CARDBACK_RED:
		combo_cardback->SetSelection(raPREFS_CARDBACK_RED);
		break;
	default:
		wxLogError(wxString::Format(wxT("Unexpected value. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	}

	check_autoplay = XRCCTRL(*this, "m_radlgprefs_playsingauto", wxCheckBox);
	check_autoplay->SetValue(conf_data.prefs_data.auto_play_single);
	check_bidbubbles = XRCCTRL(*this, "m_radlgprefs_showbidbubb", wxCheckBox);
	check_bidbubbles->SetValue(conf_data.prefs_data.show_bid_bubbles);

	event.Skip();
}

void raDlgPrefs::OnPrefsBtnApplyClick( wxCommandEvent& event ) {
	wxComboBox *combo_playcardon;
	wxComboBox *combo_cardback;
	wxCheckBox *check_autoplay;
	wxCheckBox *check_bidbubbles;
	raConfData new_conf;
	raConfig::GetInstance()->GetData(&new_conf);
	combo_playcardon = XRCCTRL(*this, "m_radlgprefs_playcardon", wxComboBox);
	switch(combo_playcardon->GetSelection()) {
	case raPREFS_PLAYCARDON_SCLICK:
		new_conf.prefs_data.play_card_on = raCONFIG_PREFS_PLAYCARDON_SCLICK;
		break;
	case raPREFS_PLAYCARDON_DCLICK:
		new_conf.prefs_data.play_card_on = raCONFIG_PREFS_PLAYCARDON_DCLICK;
		break;
	default:
		wxLogError(wxString::Format(wxT("Unexpected value. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	}

	combo_cardback = XRCCTRL(*this, "m_radlgprefs_cardback", wxComboBox);

	switch(combo_cardback->GetSelection()) {
	case raPREFS_CARDBACK_BLUE:
		new_conf.prefs_data.card_back = raCONFIG_PREFS_CARDBACK_BLUE;
		break;
	case raPREFS_CARDBACK_RED:
		new_conf.prefs_data.card_back = raCONFIG_PREFS_CARDBACK_RED;
		break;
	default:
		wxLogError(wxString::Format(wxT("Unexpected value. %s:%d"), wxT(__FILE__), __LINE__));
		break;
	}

	check_autoplay = XRCCTRL(*this, "m_radlgprefs_playsingauto", wxCheckBox);
	new_conf.prefs_data.auto_play_single = check_autoplay->GetValue();
	check_bidbubbles = XRCCTRL(*this, "m_radlgprefs_showbidbubb", wxCheckBox);
	new_conf.prefs_data.show_bid_bubbles = check_bidbubbles->GetValue();

	raConfig::GetInstance()->SetData(&new_conf);
	event.Skip();

	Destroy();
}


