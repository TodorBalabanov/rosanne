#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#define MEXP 19937
#define N (MEXP / 128 + 1)
#define N32 (N * 4)

class gmRandState {
public:
	unsigned int state_array[N32];
	int idx;
} ;


class gmRand {
public:
	static wxString PrintState();
	static void GetState(gmRandState *state);
	static void SetState(gmRandState *state);
};


