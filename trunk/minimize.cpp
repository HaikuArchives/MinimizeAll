/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "minimize.h"

#include <Beep.h>
#include <InterfaceDefs.h>
#ifdef __INTEL__
#include <iostream>
#endif
#include <Looper.h>
#include <Roster.h>
#include <string.h>
#include <vector>

// wird irgendwo vom BeOS definiert (stammt aus Tracker source)
void do_minimize_team(BRect zoomRect, team_id team, bool zoom);
void do_bring_to_front_team(BRect zoomRect, team_id app, bool zoom);

vector<thread_id>	last_threads;

// decide if this application may be maximized / minimized
bool is_valid_app( app_info &a_info ) {

	char excluded_teams[][B_MIME_TYPE_LENGTH] = {
		"", /* system teams */
		"application/x-vnd.Be-TSKB", /* Deskbar */
		"application/x-vnd.Be-APPS", /* application_server */
		"application/x-vnd.Be-input_server", /* input_server */
	};

	for (uint i=0; i<sizeof(excluded_teams)/B_MIME_TYPE_LENGTH; ++i) {
		if (strcmp(a_info.signature, excluded_teams[i])==0) {
			return false;
		}
	}
	
	return true;
}	

void minimize( min_mode min ) {

	BRect zoomRect(0, 0, 0, 0);

	app_info a_info;
	
	switch (min) {

		case min_maximize_last: {	// maximieren
			if (!last_threads.empty()) {
				
				vector<thread_id>::iterator	i = last_threads.end();
				i--;
				
				thread_id last_team = *i;
				
				be_roster->GetRunningAppInfo(last_team, &a_info);
				do_bring_to_front_team(zoomRect, a_info.team, true);
	
				last_threads.pop_back();
			}
		} break;
		
		case min_minimize_all: {	// alle Anwendungen
			int32 cookie = 0;
			team_info t_info;
		
			while (get_next_team_info(&cookie, &t_info) == B_OK) {
	
				be_roster->GetRunningAppInfo(t_info.team, &a_info);
				// only if the app can be activated, it has a window!
				team_id	team = a_info.team;
				be_roster->ActivateApp(team);
				
				if ( 	is_valid_app(a_info)
					&&	be_roster->GetActiveAppInfo(&a_info)==B_OK
					&&	a_info.team==team )
				{
					
						do_minimize_team(zoomRect, team, true);
						last_threads.push_back( team );
				}
			}
		} break;

		default: { // aktuelle Anwenung verkleinern
			
			be_roster->GetActiveAppInfo(&a_info);
			
			if (is_valid_app(a_info)) {
				do_minimize_team(zoomRect, a_info.team, true);
				last_threads.push_back( a_info.team );
			}
		}
	}
}
