#include "gmrand.h"

wxString gmRand::PrintState()
{
    wxString out;
    int i;

    out.Empty();

	out.Append(wxT("\n-Random State-\n"));
	for(i = 0; i < N32; i++)
	{
	    //out.Append(wxString::Format(wxT("seed%02d=%08X\n"), i, psfmt32[idxof(i)]));
	}
	//out.Append(wxString::Format(wxT("idx=%d\n"), idx));
	out.Append(wxT("-Random State-"));

	return out;
}

void gmRand::GetState(gmRandState *state)
{
    int i = 0;
    for(i = 0; i < N32; i++)
    {
        //state->state_array[i] = psfmt32[idxof(i)];
        //state->idx = idx;
    }
}

void gmRand::SetState(gmRandState *state)
{
    int i = 0;
    for(i = 0; i < N32; i++)
    {
        //psfmt32[idxof(i)] = state->state_array[i];
        //idx = state->idx;
    }
}

