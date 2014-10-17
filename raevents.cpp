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

#include "raevents.h"

const wxEventType raINFO_EVT = wxNewEventType();
const wxEventType raBID_EVT = wxNewEventType();
const wxEventType raUPDATE_EVT = wxNewEventType();

IMPLEMENT_DYNAMIC_CLASS(raInfoEvent, wxEvent)
//DEFINE_EVENT_TYPE(EVT_GAME_NEW)

raInfoEvent::raInfoEvent() : wxEvent( raINFO_EVT ) {
	// TODO : Use a #defined constant here. Include relevant header in this cpp
	m_cmd = -1;
}
raInfoEvent::raInfoEvent(const raInfoEvent &evt) {
	SetEventType(raINFO_EVT);
	m_cmd = evt.m_cmd;
}
void raInfoEvent::SetCommand(int cmd) {
	m_cmd = cmd;
}
int raInfoEvent::GetCommand() {
	return m_cmd;
}


IMPLEMENT_DYNAMIC_CLASS(raBidEvent, wxEvent)
//DEFINE_EVENT_TYPE(EVT_GAME_NEW)

raBidEvent::raBidEvent() : wxEvent( raBID_EVT ) {
	// TODO : Use a #defined constant here. Include relevant header in this cpp
	m_bid = -1;
}
raBidEvent::raBidEvent(const raBidEvent &evt) {
	SetEventType(raBID_EVT);
	m_bid = evt.m_bid;
}
void raBidEvent::SetBid(int bid) {
	m_bid = bid;
}
int raBidEvent::GetBid() {
	return m_bid;
}

IMPLEMENT_DYNAMIC_CLASS(raUpdateEvent, wxEvent)

raUpdateEvent::raUpdateEvent() : wxEvent( raUPDATE_EVT ) {

}
raUpdateEvent::raUpdateEvent(const raUpdateEvent &evt) {
	SetEventType(raUPDATE_EVT);
	m_msg = evt.m_msg;
}
void raUpdateEvent::SetMessage(wxString msg) {
	m_msg = msg;
}
wxString raUpdateEvent::GetMessage() {
	return m_msg;
}


