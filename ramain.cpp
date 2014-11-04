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
#include <wx/file.h>
#include <wx/utils.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/splitter.h>
#include <wx/grid.h>
#include <wx/socket.h>

#include "ramain.h"
#include "gmrand.h"
#include <time.h>

#include "main_icon_16.xpm"
#include "new_game.xpm"
#include "exit.xpm"
#include "options.xpm"
#include "rules.xpm"
#include "help.xpm"
#include "about.xpm"
#include "tile.xpm"
#include "bid_history.xpm"
#include "last_trick.xpm"

// Event table for raFrame
BEGIN_EVENT_TABLE(raFrame, wxFrame)
	EVT_MENU(raID_ABOUT, raFrame::OnAbout)
	EVT_MENU(raID_EXIT,  raFrame::OnQuit)
	EVT_MENU(raID_NEW_GAME,  raFrame::OnGameNew)
	EVT_MENU(raID_PREFERENCES,  raFrame::OnPreferences)
	EVT_MENU(raID_RULES,  raFrame::OnRules)
	EVT_MENU(raID_BID_HISTORY,  raFrame::OnAuction)
	EVT_MENU(raID_LAST_TRICK,  raFrame::OnLastTrick)
	EVT_CLOSE(raFrame::OnClose)
	EVT_SIZE(raFrame::OnSize)
	EVT_UPDATE(raFrame::OnUpdate)
END_EVENT_TABLE()

// Give wxWidgets the means to create a raApp object
IMPLEMENT_APP(raApp)

// Initialize the application
bool raApp::OnInit() {
	raConfig *config;
	raConfData conf_data;
	wxString log_file = raApp::GenerateLogFileName();

	// Open the log file for writing

	m_logfile = fopen(log_file.mb_str(), "w+");
	if(m_logfile == NULL) {
		::wxMessageBox(wxString::Format(wxT("Failed to open log file \"%s\" for writing.\nThe program will now terminate."),
										log_file.c_str()), wxT("Fatal Error!"), wxICON_ERROR);
		return false;
	}

	m_logger = new wxLogStderr(m_logfile);
	wxASSERT(m_logger);

	m_old_logger = wxLog::GetActiveTarget();

	wxLog::SetActiveTarget(m_logger);
	wxLogDebug(wxT("Logging opened."));

	// Log details such as operating system, architecture etc which if required can be used later on
	// for debugging

	raApp::LogDetailsForDebug();

	// Obtain the configuration data

	config = raConfig::GetInstance();
	config->GetData(&conf_data);

	// Randomizing the PRNG

	srand(time(0));
	wxLogMessage(wxT("SFMT PRNG initiated."));
	wxLogMessage(wxString::Format(wxT("MEXP = %d"), MEXP));
	wxLogMessage(wxString::Format(wxT("N32 = %d"), N32));
	wxLogMessage(wxT(""));

	//For usage of sockets or derived classes such as wxFTP in a secondary thread
	wxSocketBase::Initialize();

	// Initiate all handlers and then enable the ZipFsHandler. This is required because we are
	// calling the wxXmlResource::Get()->Load() from OnInit()
	wxXmlResource::Get()->InitAllHandlers();
	wxFileSystem::AddHandler(new wxZipFSHandler);

	if(!wxFile::Exists(raGUI_XRS)) {
		::wxMessageBox(wxString::Format(wxT("Compiled resource file \"%s\" does not exist."), raGUI_XRS),
					   wxT("Fatal Error!"), wxICON_ERROR);
		wxLog::SetActiveTarget(m_old_logger);
		delete m_logger;
		fclose(m_logfile);
		return false;
	}

	if(!wxXmlResource::Get()->Load(raGUI_XRS)) {
		wxLogError(wxString::Format(wxT("Failed to load xrs %s. %s:%d"), raGUI_XRS,  wxT(__FILE__), __LINE__));
		wxLog::SetActiveTarget(m_old_logger);
		delete m_logger;
		fclose(m_logfile);
		return false;
	}

	// Create the main application window
	wxLogDebug(RA_APP_FULL_VER);
	m_frame = new raFrame(RA_APP_FULL_NAME);

	// If the window dimensions have been saved from the previous run,
	// try to create the main window using the same. Othewise, try to
	// create the window of 3/4th size of the desktop
	if(
		(conf_data.app_data.x != -1) &&
		(conf_data.app_data.y != -1) &&
		(conf_data.app_data.width != -1) &&
		(conf_data.app_data.height != -1)
	) {
		m_frame->SetSize(wxRect(
							 conf_data.app_data.x,
							 conf_data.app_data.y,
							 conf_data.app_data.width,
							 conf_data.app_data.height));
	} else {
		m_frame->SetSize(
			(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 3 / 4),
			wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 3 / 4
		);
	}

	// Depending on the saved configuration, maxim
	if(conf_data.app_data.maximized)
		m_frame->Maximize();

	SetTopWindow(m_frame);
	m_frame->Show(true);

	return true;
}

int raApp::OnRun() {
	// Check for updates
	m_update = NULL;
	m_update = new raUpdate();
	if(!m_update) {
		wxLogError(wxString::Format(wxT("m_update = new raUpdate(); failed. %s:%d"), wxT(__FILE__), __LINE__));
		wxMessageBox(wxT("Failed to create an instance of the thread which checks for updates!"));
	}
	if (m_update->Create() != wxTHREAD_NO_ERROR ) {
		wxLogError(wxString::Format(wxT("m_update->Create(). %s:%d"), wxT(__FILE__), __LINE__));
		wxMessageBox(wxT("Failed to create the thread which checks for updates!"));
	}
	m_update->Run();

	wxApp::OnRun();
	return 0;
}

int raApp::OnExit() {
	// Save settings
	if(!raConfig::GetInstance()->Save()) {
		wxLogError(wxString::Format(wxT("Attempt to save settings failed. %s:%d"), wxT(__FILE__), __LINE__));
	}

	// Stop logging
	wxLogDebug(wxT("Attempting to stop logger."));

	wxLog::SetActiveTarget(m_old_logger);
	delete m_logger;
	fclose(m_logfile);

	return 0;
}

wxString raApp::GenerateLogFileName() {
	wxString out;
	wxDateTime now = wxDateTime::Now();

	out.Empty();

	out.Append(raLOG_DIR);
	out.Append(wxFileName::GetPathSeparator());
	out.Append(raLOG_FILE_PREFIX raLOG_FILE_DELIM);
	out.Append(wxString::Format(wxT("%04d"), now.GetYear()));
	out.Append(wxString::Format(wxT("%02d"), now.GetMonth() + 1));
	out.Append(wxString::Format(wxT("%02d"), now.GetDay()));
	out.Append(raLOG_FILE_DELIM);
	out.Append(wxString::Format(wxT("%02d"), now.GetHour()));
	out.Append(wxString::Format(wxT("%02d"), now.GetMinute()));
	out.Append(wxString::Format(wxT("%02d"), now.GetSecond()));
	out.Append(raLOG_FILE_DELIM);
	out.Append(wxString::Format(wxT("%lu"), wxGetProcessId()));
	out.Append(wxT(".") raLOG_FILE_EXTN);

	return out;
}

void raApp::LogDetailsForDebug() {
	wxString out;
	wxLogMessage(wxT("Program              : ") RA_APP_FULL_NAME);
	wxLogMessage(wxT("Date of compilation  : ") __DATE__ __TIME__);

#ifdef __GNUC__
	out.Empty();
	out.Append(wxT("Compiler             : GNU C/C++ "));
	out.Append(wxString::Format(wxT("%d.%d"),__GNUC__, __GNUC_MINOR__));
	wxLogMessage(out);
#endif

	out.Empty();
	out.Append(wxT("Operating System     : "));
	out.Append(::wxGetOsDescription());
	if(::wxIsPlatform64Bit() == true) {
		out.Append(wxT("(64 bit)"));
	}
	wxLogMessage(out);

	out.Empty();
	out.Append(wxT("Endianness           : "));
	if(::wxIsPlatformLittleEndian() == true) {
		out.Append(wxT("Little Endian"));
	} else {
		out.Append(wxT("Big Endian"));
	}
	wxLogMessage(out);
	wxLogMessage(wxT(""));
}

void raFrame::OnAbout(wxCommandEvent& event) {
	raDlgAbout about;
	if(!wxXmlResource::Get()->LoadDialog(&about, this, wxT("raDlgAbout"))){
		wxLogError(wxString::Format(wxT("Attempt to save settings failed. %s:%d"), wxT(__FILE__), __LINE__));
	}
	about.ShowModal();
}

void raFrame::OnQuit(wxCommandEvent& event) {
	// Destroy the frame
	Close();
}

void raFrame::OnGameNew(wxCommandEvent& event) {
	m_game->NewGame(raGetRandPlayer());
}

void raFrame::OnClose(wxCloseEvent& event) {
	raConfig *config;
	raConfData conf_data;
	wxPoint pt;
	wxSize sz;

	// Get confirmation from the user before
	// closing the appliation
	if(wxMessageBox(wxT("Exit application?"),
					wxT("Confirm"), wxYES_NO | wxICON_QUESTION) == wxNO) {
		event.Veto();
		return;
	}

	// Save the frame location and size
	// only if the window is not minimized or iconized
	if(!IsIconized()) {
		config = raConfig::GetInstance();
		config->GetData(&conf_data);

		if(IsMaximized()) {
			conf_data.app_data.maximized = true;
		} else {
			pt = wxPoint(0, 0);
			sz = wxSize(0, 0);
			pt = GetPosition();
			sz = GetSize();

			conf_data.app_data.x = pt.x;
			conf_data.app_data.y = pt.y;
			conf_data.app_data.width = sz.GetWidth();
			conf_data.app_data.height = sz.GetHeight();

			conf_data.app_data.maximized = false;
		}

		config->SetData(&conf_data);
	}

	event.Skip();
}

void raFrame::OnPreferences(wxCommandEvent& event) {
	if(!ShowPreferences()) {
		wxLogError(wxString::Format(wxT("ShowPreferences() failed. %s:%d"), wxT(__FILE__), __LINE__));
	}
}

void raFrame::OnRules(wxCommandEvent& event) {
	if(!ShowRules()) {
		wxLogError(wxString::Format(wxT("ShowRules() failed. %s:%d"), wxT(__FILE__), __LINE__));
	}
}

void raFrame::OnAuction(wxCommandEvent& event) {
	if(!m_game->ShowAuction()) {
		wxLogError(wxString::Format(wxT("ShowAuction() failed. %s:%d"), wxT(__FILE__), __LINE__));
	}
}

void raFrame::OnLastTrick(wxCommandEvent& event) {
	if(!m_game->ShowLastTrick()) {
		wxLogError(wxString::Format(wxT("ShowLastTricks() failed. %s:%d"), wxT(__FILE__), __LINE__));
	}
}

void raFrame::OnSize(wxSizeEvent& event) {
	if(m_split_main) {
		m_split_main->Refresh();
		m_split_main->Update();
	}
	event.Skip();
}

void raFrame::OnUpdate(raUpdateEvent& event) {
	wxMessageBox(event.GetMessage());
	event.Skip();
}

bool raFrame::ShowPreferences() {
	raDlgPrefs dlg_prefs;

	if(!wxXmlResource::Get()->LoadDialog(&dlg_prefs, this, wxT("raDlgPrefs"))) {
		wxLogError(wxString::Format(wxT("Attempt to save settings failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	dlg_prefs.ShowModal();

	if(!m_game->ReloadFromConfig()) {
		wxLogError(wxString::Format(wxT("ReloadFromConfig failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	return true;
}

bool raFrame::ShowRules() {
	raDlgRules dlg_rules;

	if(!wxXmlResource::Get()->LoadDialog(&dlg_rules, this, wxT("raDlgRules"))) {
		wxLogError(wxString::Format(wxT("Attempt to save settings failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	dlg_rules.ShowModal();

	if(!m_game->ReloadFromConfig()) {
		wxLogError(wxString::Format(wxT("ReloadFromConfig failed. %s:%d"), wxT(__FILE__), __LINE__));
		return false;
	}
	return true;
}

raFrame::raFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
	wxBitmap tile(tile_xpm);
	wxBitmap bmp_new_game(new_game_xpm);
	wxBitmap bmp_exit(exit_xpm);
	wxBitmap bmp_options(options_xpm);
	wxBitmap bmp_rules(rules_xpm);
	wxBitmap bmp_bid_history(bid_history_xpm);
	wxBitmap bmp_last_trick(last_trick_xpm);
	wxBitmap bmp_help(help_xpm);
	wxBitmap bmp_about(about_xpm);

	wxMenuBar *menu_bar = NULL;
	wxMenu *game_menu = NULL;
	wxMenu *help_menu = NULL;
	wxMenu *opt_menu = NULL;
	wxMenu *view_menu = NULL;

	wxMenuItem *game_new = NULL;
	wxMenuItem *game_exit = NULL;

	wxMenuItem *opt_prefs = NULL;
	wxMenuItem *opt_rules = NULL;

	wxMenuItem *view_bid_history = NULL;
	wxMenuItem *view_last_trick = NULL;

	wxMenuItem *help_conts = NULL;
	wxMenuItem *help_about = NULL;

	wxToolBar *tool_bar;

	wxStatusBar *status_bar;

	m_split_main = NULL;

	// Set the frame icon
	SetIcon(wxIcon(main_icon_16_xpm));

	game_menu = new wxMenu;
	help_menu = new wxMenu;
	opt_menu = new wxMenu;
	view_menu = new wxMenu;

	game_new = new wxMenuItem(game_menu, raID_NEW_GAME, wxT("&New"));
	game_new->SetBitmap(bmp_new_game);
	game_menu->Append(game_new);
	game_exit = new wxMenuItem(game_menu, raID_EXIT, wxT("E&xit"));
	game_exit->SetBitmap(bmp_exit);
	game_menu->Append(game_exit);

	opt_prefs = new wxMenuItem(opt_menu, raID_PREFERENCES, wxT("&Preferences"));
	opt_prefs->SetBitmap(bmp_options);
	opt_menu->Append(opt_prefs);
	opt_rules = new wxMenuItem(opt_menu, raID_RULES, wxT("&Rules"));
	opt_rules->SetBitmap(bmp_rules);
	opt_menu->Append(opt_rules);

	view_bid_history = new wxMenuItem(view_menu, raID_BID_HISTORY, wxT("&Auction"));
	view_bid_history->SetBitmap(bmp_bid_history);
	view_menu->Append(view_bid_history);
	view_last_trick = new wxMenuItem(view_menu, raID_LAST_TRICK, wxT("&Last Trick"));
	view_last_trick->SetBitmap(bmp_last_trick);
	view_menu->Append(view_last_trick);

	help_conts = new wxMenuItem(help_menu, wxID_HELP_CONTENTS, wxT("&Contents"));
	help_conts->SetBitmap(bmp_help);
	help_menu->Append(help_conts);
	help_about = new wxMenuItem(help_menu, raID_ABOUT, wxT("&About"));
	help_about->SetBitmap(bmp_about);
	help_menu->Append(help_about);

	// Now append the freshly created menu to the menu bar...
	menu_bar = new wxMenuBar();
	menu_bar->Append(game_menu, wxT("&Game"));
	menu_bar->Append(opt_menu, wxT("&Options"));
	menu_bar->Append(view_menu, wxT("&View"));
	menu_bar->Append(help_menu, wxT("&Help"));

	// ... and attach this menu bar to the frame
	SetMenuBar(menu_bar);

	help_conts->Enable(false);

	// Create the Tool Bar
	tool_bar = new wxToolBar(this, wxID_ANY,
							 wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER|wxTB_FLAT);
	tool_bar->SetToolBitmapSize(wxSize(16,16));

	tool_bar->AddTool(raID_NEW_GAME, bmp_new_game, wxT("New Game"));
	tool_bar->AddTool(raID_EXIT, bmp_exit, wxT("Exit"));
	tool_bar->AddSeparator();
	tool_bar->AddTool(raID_PREFERENCES, bmp_options, wxT("Preferences"));
	tool_bar->AddTool(raID_RULES, bmp_rules, wxT("Rules"));
	tool_bar->AddSeparator();
	tool_bar->AddTool(raID_BID_HISTORY, bmp_bid_history, wxT("Auction"));
	tool_bar->AddTool(raID_LAST_TRICK, bmp_last_trick, wxT("Last Trick"));
	tool_bar->AddSeparator();
	tool_bar->AddTool(wxID_ANY, bmp_help, wxT("Help"));
	tool_bar->AddTool(raID_ABOUT, bmp_about, wxT("About"));
	tool_bar->Realize();
	this->SetToolBar(tool_bar);

	// Create the main splitter control
	m_split_main = new wxSplitterWindow(this);
	m_split_main->Refresh();
	m_split_main->SetSashGravity(0.0);
	m_info = new raInfo(m_split_main);

	m_game = new raGamePanel(m_split_main);
	m_game->SetTile(&tile);

	m_game->SetInfoPanel(m_info);
	m_info->SetGamePanel(m_game);

	m_split_main->SplitVertically(m_info, m_game, 160);

	// Create a status bar
	status_bar = CreateStatusBar();
	status_bar->SetFieldsCount(raSBAR_FIELDS);
	int status_widths[raSBAR_FIELDS] = {-1, 200};
	status_bar->SetStatusWidths(raSBAR_FIELDS, status_widths);

	// Start a new game but do not deal immediately
	m_game->NewGame(raGetRandPlayer(), false);
}
