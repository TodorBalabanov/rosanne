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

#ifndef _RACOMMON_H_
#define _RACOMMON_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//#include "bh/bh.h"

// Including images
//#include "main_icon_16.xpm"
//#include "new_game.xpm"
//#include "exit.xpm"
//#include "options.xpm"
//#include "help.xpm"



#define RA_APP_MAJOR_VER wxT("1")
#define RA_APP_MINOR_VER wxT("0")
#define RA_APP_REL_TYPE wxT("b")
#define RA_APP_REL_TYPE_VER wxT("1")
#define RA_APP_NAME wxT("Rosanne")
#define RA_APP_AUTHOR wxT("Vipin Cherian")

#define RA_APP_FULL_VER RA_APP_MAJOR_VER wxT(".") RA_APP_MINOR_VER RA_APP_REL_TYPE RA_APP_REL_TYPE_VER


#define RA_APP_FULL_NAME RA_APP_NAME wxT(" ") RA_APP_FULL_VER


#define ra_APP_URL wxT("http://rosanne.sourceforge.net")

#define raBID_INVALID -2
#define raGetRandPlayer() (rand() % gmTOTAL_PLAYERS)

// For testing purposes
#define raTEST_DATA_FILE wxT("ra_test_data.ini")
#define raTEXT_SEED wxT("rand/seed")
#define raTEXT_IDX wxT("rand/idx")
#define raTEXT_DEALER wxT("deal/dealer")
#define raTEXT_DEAL_ROUND wxT("deal_round")

// Colours
#define raCLR_HEAD_DARK (wxColour(0, 92, 133))
#define raCLR_HEAD_LITE (wxColour(136, 219, 255))

#define raCLR_BLUE_DARK wxColour(129, 203, 255)
#define raCLR_BLUE_LITE wxColour(168, 219, 255)
#define raCLR_PURP_DARK wxColour(188, 174, 255)
#define raCLR_PURP_LITE wxColour(212, 204, 255)

#define raCLR_BUBB_FILL wxColour(173, 255, 135)

//#define raCLR_INSTR wxColour(144, 0, 255)
#define raCLR_INSTR wxColour(255, 0, 144)
//#define raCLR_INSTR (*wxRED)

#define raARROW_WIDTH 16
#define raBUBB_ARROW_WIDTH 12
//#define raBUBB_EDGE_WIDTH 5
//#define raBUBB_CORNER_WIDTH 5
#define raBUBB_UNIT_MIN 5
#define raBUBB_MIN_WIDTH 120
#define raBUBB_MIN_HEIGHT 40
#define raBUBB_ARROW_OVERLAP 3
#define raBUBB_ARROW_PROTUN (raBUBB_ARROW_WIDTH - raBUBB_ARROW_OVERLAP)

enum {
	raSBARPOS_GEN = 0,
	raSBARPOS_CLOCK
};

#define raStatusBar ((wxFrame *)(wxTheApp->GetTopWindow()))->GetStatusBar()

#define raTEXT_CLOCKWISE wxT("Clockwise")
#define raTEXT_ANTICLOCKWISE wxT("Anti-lockwise")

#endif

