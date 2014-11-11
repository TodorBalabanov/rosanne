#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "gmRandState.h"

class gmRand {
public:
	static wxString PrintState();
	static void GetState(gmRandState *state);
	static void SetState(gmRandState *state);
};


