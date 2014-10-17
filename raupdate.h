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

#ifndef _RAUPDATE_H_
#define _RAUPDATE_H_

#include "racommon.h"
#include "raevents.h"
#include "wx/thread.h"
//#include "wx/filesys.h"
//#include "wx/fs_inet.h"
#include "wx/protocol/http.h"
#include "wx/url.h"

#define raUPDATE_VER wxT("1")
#define raUPDATE_URL wxT("http://rosanne.sourceforge.net/ver.txt")
//#define raUPDATE_APPURL "http://rosanne.sourceforge.net"


class raUpdate : public wxThread {
public:
	raUpdate();
	virtual void* Entry();
private:
	//wxString m_new_ver;
	//wxFSFile *m_f;
	//wxFileSystem *m_fs;
	int CheckForUpdate(wxString *new_ver = NULL);
	// Disallow copy constructor/assignment operators
	raUpdate(const raUpdate &);
	raUpdate & operator=(const raUpdate &);

};

#endif
