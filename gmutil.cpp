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


// Part of the code borrowed from
// http://graphics.stanford.edu/~seander/bithacks.html

#include "gmutil.h"

//int gmUtil::m_value_trans[] = {5, -1, -1, -1, -1, -1, 0, 1, 6, 4, 7, 2, 3};
int gmUtil::m_value_trans[] = {6, 7, 11, 12, 9, 0, 8, 10};
wxString gmUtil::m_suits[] = {wxT("C"), wxT("D"), wxT("H"), wxT("S")};
wxString gmUtil::m_values[] = {wxT("7"), wxT("8"), wxT("Q"), wxT("K"), wxT("10"), wxT("A"), wxT("9"), wxT("J")};
wxString gmUtil::m_short_locs[] = {wxT("S"), wxT("W"), wxT("N"), wxT("E")};
wxString gmUtil::m_long_locs[] = {wxT("South"), wxT("West"), wxT("North"), wxT("East")};
unsigned long gmUtil::m_suit_mask[] = {0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000};
unsigned long gmUtil::m_suit_rs[] = {0, 8, 16, 24};
int gmUtil::m_points[] = {0, 0, 0, 0, 1, 1, 2, 3};
int gmUtil::m_total_points[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7
};
wxString gmUtil::m_short_teams[] = {wxT("N/S"), wxT("E/W"), wxT("N/S"), wxT("E/W")};

unsigned int gmUtil::BitsSetTable256[] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

int gmUtil::LogTable256[] = {
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

//Fisher-Yates shuffle
//http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
//
//(For a zero-based array)
//To shuffle an array a of n elements:
//   for i from n - 1 downto 1 do
//         j ? random integer with 0 <= j <= i
//         exchange a[j] and a[i]

void gmUtil::ShuffleArray(int *array, unsigned long n) {
	if (n <= 1) {
		return;
	}

	unsigned long i;
	for (i = n - 1; i >= 1; i--) {
		// TODO : Non linear distribution. To be corrected.
		unsigned long j = ((unsigned long)rand()) % (i + 1);
		int t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}

wxString gmUtil::PrintLong(unsigned long cards) {
	wxString out, final;

	//wxLogDebug(wxString::Format("Cards = %lu", cards));

	out = _("");
	//wxLogDebug(wxString::Format("%s%s", m_suits[highest / 8], m_values[highest % 8]));
	for(int i = 0; i < 32; i++) {
		if(cards & (1 << i))
			out = out + m_suits[i / 8] + m_values[i % 8] + _(",");
	}
	if(!out.IsEmpty()) {
		final = out.Left(out.Length() - 1);
	}

	return final;
}
wxString gmUtil::PrintHands(unsigned long *hands) {
	wxString ret_val;
	wxString final;
	wxString out;
	unsigned long temp;

	ret_val.Clear();

	// Print North first
	//wxLogMessage(SPACES20 + m_long_locs[2]);
	ret_val.Append(SPACES20 + m_long_locs[2]);
	ret_val.Append(wxT("\n"));

	for (int i = 0; i < 4; i++) {
		temp = (hands[2] & m_suit_mask[i]) >> m_suit_rs[i];
		out = wxString::Format(wxT("%s - "), m_suits[i].c_str());
		for(int j = 7; j >= 0; j--) {
			if(temp & (1 << j))
				out = out + m_values[j % 8] + _(",");
		}

		final = out.Left(out.Length() - 1);
		//wxLogMessage(SPACES20 + final);
		ret_val.Append(SPACES20 + final);
		ret_val.Append(wxT("\n"));
	}

	// Print East and West in the same line :D
	//wxLogMessage(wxString::Format("%-40s%-40s", m_long_locs[1], m_long_locs[3]));
	ret_val.Append(wxString::Format(wxT("%-40s%-40s"), m_long_locs[1].c_str(), m_long_locs[3].c_str()));
	ret_val.Append(wxT("\n"));

	for (int i = 0; i < 4; i++) {
		temp = (hands[1] & m_suit_mask[i]) >> m_suit_rs[i];
		out = wxString::Format(wxT("%s - "), m_suits[i].c_str());
		//for(j = 0; j < 8; j++)
		for(int j = 7; j >= 0; j--) {
			if(temp & (1 << j))
				out = out + m_values[j % 8] + _(",");
		}

		final = wxString::Format(wxT("%-40s"), out.Left(out.Length() - 1).c_str());

		temp = (hands[3] & m_suit_mask[i]) >> m_suit_rs[i];
		out = wxString::Format(wxT("%s - "), m_suits[i].c_str());
		//for(j = 0; j < 8; j++)
		for(int j = 7; j >= 0; j--) {
			if(temp & (1 << j))
				out = out + m_values[j % 8] + _(",");
		}

		final += wxString::Format(wxT("%-40s"), out.Left(out.Length() - 1).c_str());

		//wxLogMessage(final);
		ret_val.Append(final);
		ret_val.Append(wxT("\n"));
	}

	// Finally print South
	//wxLogMessage(SPACES20 + m_long_locs[0]);
	ret_val.Append(SPACES20 + m_long_locs[0]);
	ret_val.Append(wxT("\n"));

	for (int i = 0; i < 4; i++) {
		temp = (hands[0] & m_suit_mask[i]) >> m_suit_rs[i];
		out = wxString::Format(wxT("%s - "), m_suits[i].c_str());
		//for(j = 0; j < 8; j++)
		for(int j = 7; j >= 0; j--) {
			if(temp & (1 << j))
				out = out + m_values[j % 8] + _(",");
		}

		final = out.Left(out.Length() - 1);
		//wxLogMessage(SPACES20 + final);
		ret_val.Append(SPACES20 + final);
		ret_val.Append(wxT("\n"));
	}
	return ret_val;
}

int gmUtil::GetCardIndex(wxString text) {
	wxString suit, value;
	int x, y;
	suit = text.Left(1);
	//wxPrintf(suit + "\n");
	value = text.Mid(1);
	//wxPrintf(value + "\n");

	if(!suit.CmpNoCase(wxT("C")))
		x = 0;
	else if(!suit.CmpNoCase(wxT("D")))
		x = 1;
	else if(!suit.CmpNoCase(wxT("H")))
		x = 2;
	else if(!suit.CmpNoCase(wxT("S")))
		x = 3;
	else
		return -1;

	if(!value.CmpNoCase(wxT("7")))
		y = 0;
	else if(!value.CmpNoCase(wxT("8")))
		y = 1;
	else if(!value.CmpNoCase(wxT("Q")))
		y = 2;
	else if(!value.CmpNoCase(wxT("K")))
		y = 3;
	else if(!value.CmpNoCase(wxT("10")))
		y = 4;
	else if(!value.CmpNoCase(wxT("A")))
		y = 5;
	else if(!value.CmpNoCase(wxT("9")))
		y = 6;
	else if(!value.CmpNoCase(wxT("J")))
		y = 7;
	else
		return -1;

	return (x * 8) + y;
}
bool gmUtil::SetStatusText(const wxString& text, int i) {
	wxFrame *main_frame;
	wxStatusBar *status_bar;

	main_frame = NULL;
	main_frame = (wxFrame *)wxTheApp->GetTopWindow();
	if(!main_frame)
		return false;

	status_bar = NULL;
	status_bar = main_frame->GetStatusBar();
	if(!status_bar)
		return false;

	status_bar->SetStatusText(text, i);
	status_bar->Update();
	return true;
}



unsigned long gmUtil::CountBitsSet(unsigned long v) {
	return BitsSetTable256[v & 0xff] +
		   BitsSetTable256[(v >> 8) & 0xff] +
		   BitsSetTable256[(v >> 16) & 0xff] +
		   BitsSetTable256[v >> 24];
	//unsigned int const w = v - ((v >> 1) & 0x55555555);                    // temp
	//unsigned int const x = (w & 0x33333333) + ((w >> 2) & 0x33333333);     // temp
	//unsigned int const c = ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
	//return c;
}

unsigned long gmUtil::HighestBitSet(unsigned long v) {
	register unsigned long t, tt; // temporaries
	if (tt = v >> 16) {
		return (t = v >> 24) ? 24 + LogTable256[t] : 16 + LogTable256[tt & 0xFF];
	} else {
		return (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
	}
}

