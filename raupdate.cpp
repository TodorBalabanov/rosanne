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

#include "raupdate.h"

raUpdate::raUpdate() {
}

void* raUpdate::Entry() {
	wxString new_var = wxT("");

	int ret_val = CheckForUpdate(&new_var);
	wxASSERT(ret_val <= 2);

	if(ret_val < 0) {
		wxLogError(wxString::Format(wxT("addr.GetError() returned error. %s:%d"), wxT(__FILE__), __LINE__));
	} else if(ret_val == 0) {
		wxLogMessage(wxT("Check for update done successfully"));
	} else if(ret_val > 0) {
		wxString msg = wxT("");
		msg.Append(wxT("A new version "));
		if(!new_var.IsEmpty()) {
			msg.Append(new_var);
			msg.Append(wxT(" "));
		}
		msg.Append(wxT("is available"));
		msg.Append(wxT("."));
		msg.Append(wxT("\n"));
		msg.Append(wxT("Please download from "));
		msg.Append(ra_APP_URL);
		msg.Append(wxT("."));

		wxFrame *main_frame;
		main_frame = NULL;
		main_frame = (wxFrame *)wxTheApp->GetTopWindow();
		if(main_frame) {
			raUpdateEvent update_event;
			update_event.SetMessage(msg);
			wxLogMessage(update_event.GetMessage());
			main_frame->GetEventHandler()->AddPendingEvent(update_event);
		} else {
			wxLogError(wxString::Format(wxT("main_frame is null. %s:%d"), wxT(__FILE__), __LINE__));
		}
	}

	return NULL;
}

int raUpdate::CheckForUpdate(wxString *new_ver) {
	size_t size;
	wxChar *data = NULL;
	wxInputStream *in;
	wxString *str;
	wxURL addr(raUPDATE_URL);
	wxHTTP *http;
	wxURLError err;
	int pipe_pos;
	wxString temp;

	err = addr.GetError();
	if ( err != wxURL_NOERR ) {
		wxLogError(wxString::Format(wxT("addr.GetError() returned error. %s:%d"), wxT(__FILE__), __LINE__));
		return -1;
	}

	http = wxDynamicCast ( &addr.GetProtocol(), wxHTTP );
	if ( http ) {
		http->SetHeader ( wxT("Pragma"), wxT("no-cache") );
	}
	in = addr.GetInputStream();
	if(!in) {
		wxLogError(wxString::Format(wxT("Attempt to read from wxInputStream failed. %s:%d"), wxT(__FILE__), __LINE__));
		return -1;
	}
	size = in->GetSize();

	data = new wxChar[size + 1];
	if(!data) {
		wxLogError(wxString::Format(wxT("Attempt allocate memory for data. %s:%d"), wxT(__FILE__), __LINE__));
		return -1;
	}
	if (!in->Read(data, size)) {
		if(data)
			delete [] data;

		wxLogError(wxString::Format(wxT("Attempt to read from wxInputStream failed. %s:%d"), wxT(__FILE__), __LINE__));
		return -1;
	} else {
		delete in;

		data[size] = '\0';
		str = new wxString(data);

		if(data) {
			delete [] data;
		}

		// Get the version of the version information
		// This is the first field in the pipe delimited input
		pipe_pos = str->Find('|');
		if(pipe_pos == -1) {
			wxLogError(wxString::Format(wxT("str->Find failed. %s:%d"), wxT(__FILE__), __LINE__));
			delete str;
			return -1;
		}

		temp = str->Left(pipe_pos);
		if(temp.IsEmpty()) {
			wxLogError(wxString::Format(wxT("Empty version or version information. %s:%d"), wxT(__FILE__), __LINE__));
			delete str;
			return -1;
		}

		// Check if the obtained version of version information is
		// compatible with this class
		if(temp.CmpNoCase(raUPDATE_VER)) {
			wxLogDebug(temp);
			wxLogDebug(raUPDATE_VER);
			if(new_ver)
				*new_ver = wxT("");
			delete str;
			return 2;
		}

		temp = str->Mid(pipe_pos + 1);
		temp.Trim();
		// Check whether the versions are different
		if(temp.CmpNoCase(RA_APP_FULL_VER)) {
			wxLogDebug(temp);
			wxLogDebug(RA_APP_FULL_VER);
			if(new_ver)
				*new_ver = temp;
			delete str;
			return 1;
		}
		delete str;
	}

	return 0;
}
