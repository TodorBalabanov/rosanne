#ifndef RAAPP_H_
#define RAAPP_H_

#include <stdio.h>

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "raupdate.h"

class raFrame;

// Declare the application class
class raApp : public wxApp {
private:
	FILE *m_logfile;
	wxLogStderr *m_logger;
	wxLog *m_old_logger;
	raUpdate *m_update;
	raFrame *m_frame;
	// Disallow copy constructor/assignment operators
	//raApp(const raApp &);
	raApp & operator=(const raApp &);
public:
	// Called on application startup
	virtual bool OnInit();
	virtual int OnRun();
	virtual int OnExit();

	static wxString GenerateLogFileName();
	static void LogDetailsForDebug();
};

#endif
