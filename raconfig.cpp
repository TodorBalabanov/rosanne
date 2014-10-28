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

#include "raconfig.h"

raConfig *raConfig::s_instance = NULL;
wxMutex raConfig::s_mutex;

//
// Public methods
//

raConfig *raConfig::GetInstance() {
	// Double checked locking before creating an instance
	if(!s_instance) {
		wxMutexLocker lock(s_mutex);
		if(!lock.IsOk()) {
			wxLogError(wxString::Format(wxT("Failed to acquire mutex lock. %s:%d"), wxT(__FILE__), __LINE__));
			return NULL;
		}
		if(!s_instance)
			Create();
	}

	return s_instance;
}

bool raConfig::Save() {
	if(!m_config->Write(raCONFPATH_APP_DATA_X, m_data.app_data.x)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_APP_DATA_Y, m_data.app_data.y)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_APP_DATA_WIDTH, m_data.app_data.width)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_APP_DATA_HEIGHT, m_data.app_data.height)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_APP_DATA_MAX, m_data.app_data.maximized)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_GAME_DATA_CLOCK, m_data.game_data.clockwise)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_GAME_DATA_MINBID3, m_data.game_data.min_bid3)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_GAME_DATA_WAIVERULE4, m_data.game_data.waive_rule4)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_GAME_DATA_SLUFFJACKS, m_data.game_data.sluff_jacks)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Write(raCONFPATH_PREFS_PLAYCARDON, m_data.prefs_data.play_card_on)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	if(!m_config->Write(raCONFPATH_PREFS_CARDBACK, m_data.prefs_data.card_back)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	if(!m_config->Write(raCONFPATH_PREFS_AUTOPLAYSINGLE, m_data.prefs_data.auto_play_single)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	if(!m_config->Write(raCONFPATH_PREFS_BIDBUBBLES, m_data.prefs_data.show_bid_bubbles)) {
		wxLogError(wxString::Format(wxT("m_config->Write() failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	return true;
}

void raConfig::GetData(raConfData *data) {
	wxMutexLocker lock(s_mutex);
	*data = m_data;
}

bool raConfig::SetData(raConfData *data) {
	wxMutexLocker lock(s_mutex);
	// TODO : Sanity check incoming data
	m_data = *data;
	return true;
}

//
// Private constructor/destructor
//

raConfig::raConfig() {
	m_data.app_data.x = 0;
	m_data.app_data.y = 0;
	m_data.app_data.width = 0;
	m_data.app_data.height = 0;
	m_data.app_data.maximized = false;
	m_data.game_data.clockwise = false;
	m_data.game_data.min_bid3 = 0;
	m_data.game_data.waive_rule4 = false;
	m_data.game_data.sluff_jacks = false;
	m_data.prefs_data.play_card_on = 0;
	m_data.prefs_data.card_back = 0;
	m_data.prefs_data.auto_play_single = false;
	m_data.prefs_data.show_bid_bubbles = false;
	m_config = new wxConfig(RA_APP_NAME);

	// If the application is being run for the first time,
	// configuration data may not be present. Create it.

	// Attempt to load the data from the configuration repository
	if(!Load()) {
		// If load failed, the application is being run for the first time
		// Save default settings
		SetDefaultValues(&m_data);
		Save();
	}
}
raConfig::~raConfig() {
	delete m_config;
}

//
// Private methods
//

void raConfig::Create() {
	static raConfig the_instance;
	s_instance = &the_instance;
}

void raConfig::SetDefaultValues(raConfData *data) {
	data->app_data.x = -1;
	data->app_data.y = -1;
	data->app_data.width = -1;
	data->app_data.height = -1;
	data->app_data.maximized = false;

	data->game_data.clockwise = true;
	data->game_data.min_bid3 = 23;
	data->game_data.waive_rule4 = false;
	data->game_data.sluff_jacks = true;

	data->prefs_data.play_card_on = raCONFIG_PREFS_PLAYCARDON_SCLICK;
	data->prefs_data.card_back = raCONFIG_PREFS_CARDBACK_BLUE;
	data->prefs_data.auto_play_single = true;
	data->prefs_data.show_bid_bubbles = true;

	return;
}

bool raConfig::Load() {
	if(!m_config->Read(raCONFPATH_APP_DATA_X, &m_data.app_data.x)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_APP_DATA_Y, &m_data.app_data.y)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_APP_DATA_WIDTH, &m_data.app_data.width)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_APP_DATA_HEIGHT, &m_data.app_data.height)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_APP_DATA_MAX, &m_data.app_data.maximized)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_GAME_DATA_CLOCK, &m_data.game_data.clockwise)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_GAME_DATA_MINBID3, &m_data.game_data.min_bid3)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_GAME_DATA_WAIVERULE4, &m_data.game_data.waive_rule4)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_GAME_DATA_SLUFFJACKS, &m_data.game_data.sluff_jacks)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}

	if(!m_config->Read(raCONFPATH_PREFS_PLAYCARDON, &m_data.prefs_data.play_card_on)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	if(!m_config->Read(raCONFPATH_PREFS_CARDBACK, &m_data.prefs_data.card_back)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	if(!m_config->Read(raCONFPATH_PREFS_AUTOPLAYSINGLE, &m_data.prefs_data.auto_play_single)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	if(!m_config->Read(raCONFPATH_PREFS_BIDBUBBLES, &m_data.prefs_data.show_bid_bubbles)) {
		wxLogError(wxString::Format(wxT("m_config->Read failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}


	return true;
}

