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

#ifndef _RACONFIG_H_
#define _RACONFIG_H_

#include "racommon.h"
#include "wx/config.h"

#define raCONFPATH_APP_DATA_X wxT("application/x")
#define raCONFPATH_APP_DATA_Y wxT("application/y")
#define raCONFPATH_APP_DATA_WIDTH wxT("application/width")
#define raCONFPATH_APP_DATA_HEIGHT wxT("application/height")
#define raCONFPATH_APP_DATA_MAX wxT("application/maximized")

#define raCONFPATH_GAME_DATA_CLOCK wxT("game/clockwise")
#define raCONFPATH_GAME_DATA_MINBID3 wxT("game/minbid3")
#define raCONFPATH_GAME_DATA_WAIVERULE4 wxT("game/waiverule4")
#define raCONFPATH_GAME_DATA_SLUFFJACKS wxT("game/sluffjacks")

#define raCONFPATH_PREFS_PLAYCARDON wxT("preferences/playcardon")
#define raCONFPATH_PREFS_CARDBACK wxT("preferences/cardback")
#define raCONFPATH_PREFS_AUTOPLAYSINGLE wxT("preferences/autoplaysingle")
#define raCONFPATH_PREFS_BIDBUBBLES wxT("preferences/bidbubbles")

#define raCONFIG_PREFS_PLAYCARDON_SCLICK 0
#define raCONFIG_PREFS_PLAYCARDON_DCLICK 1
#define raCONFIG_PREFS_CARDBACK_BLUE 0
#define raCONFIG_PREFS_CARDBACK_RED 1

typedef struct tag_RA_CONF_APP_DATA
{
	int x;
	int y;
	int width;
	int height;
	bool maximized;
} raConfAppData;

typedef struct tag_RA_CONF_GAME_DATA
{
	bool clockwise;
	int min_bid3;
	bool waive_rule4;
	bool sluff_jacks;
} raConfGameData;

typedef struct tag_RA_CONF_PREFS_DATA
{
	int play_card_on;
	int card_back;
	bool auto_play_single;
	bool show_bid_bubbles;
} raConfPrefsData;

typedef struct tag_RA_CONF_DATA
{
	raConfAppData app_data;
	raConfGameData game_data;
	raConfPrefsData prefs_data;
} raConfData;

class raConfig
{
public:
	static raConfig *GetInstance();
	bool Save();
	void GetData(raConfData *data);
	bool SetData(raConfData *data);
	// Configure to make the destructor public with Visual Studio 6 - stupid compiler
#ifdef _MSC_VER
	#if _MSC_VER <= 1200
	virtual ~raConfig();
	#endif
#endif

private:

	// Disallow copy constructor/assignment operators
	raConfig(const raConfig &);
    raConfig & operator=(const raConfig &);

	wxConfig *m_config;
	raConfData m_data;

	raConfig();
//#ifndef __VISUALC6__
//	virtual ~raConfig();
//#endif
#ifdef _MSC_VER
	#if _MSC_VER > 1200
	virtual ~raConfig();
	#endif
#else
	virtual ~raConfig();
#endif
	static void Create();
	static raConfig *s_instance;
	static wxMutex s_mutex;

	static void SetDefaultValues(raConfData *data);
	bool Load();

};
#endif
