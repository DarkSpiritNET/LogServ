/* NeoStats - IRC Statistical Services 
** Copyright (c) 1999-2006 Adam Rutter, Justin Hammond, Mark Hetherington
** http://www.neostats.net/
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
**  USA
**
** NeoStats CVS Identification
** $Id: ss_help.c 1086 2004-12-12 20:15:54Z Mark $
*/

#include "neostats.h"

const char *ls_about[] = {
	"\2LogServ\2 provides a channel logging service that",
	"can be used to generate statistical information.",
	NULL
};

const char *ls_help_add[] = {
	"Add channel to log",
	"Syntax: \2ADD \37<#channel name> <PUBLIC|PRIVATE> [url]\37\2",
	"",
	"Adds a channel to LogServ for Monitoring. ",
	"Use PUBLIC to announce to the channel that logging has started",
	"or PRIVATE to avoid this announcement.",
	"You can provide an optional URL to display to users when they join",
	"the channel showing where users can view channel statistics",
	NULL
};

const char *ls_help_del[] = {
	"Stop logging channel",
	"Syntax: \2DEL \37<#channel name>\37\2",
	"",
	"Deletes a logged channel from LogServ",
	"Requires the channel name",
	NULL
};

const char *ls_help_list[] = {
	"List current channel log list",
	"Syntax: \2LIST\2",
	"",
	"Lists the channels that LogServ is currently logging",
	NULL
};

const char *ls_help_url[] = {
	"Change channel URL",
	"Syntax: \2URL \37<#channel name> <url>\37\2",
	"",
	"Modifies the URL for the stats on that channel",
	NULL
};

const char *ls_help_status[] = {
	"Simple LogServ Stats",
	"Syntax: \2STATUS\2",
	"",
	"Displays some simple status about LogServ",
	NULL
};

const char *ls_help_set_logtype[] = {
	"\2LOGTYPE\2 <type>",
	"",
	"Modifies the logfile format that LogServ uses.",
	"The options are:",
	"\2 1\2 - EggDrop Format",
	"\2 2\2 - Mirc Format",
	"\2 3\2 - Xchat Format",
	"",
	"You should restart NeoStats, or reload the LogServ module after changing",
	"the log file format so new log files will be created",
	NULL
};

const char *ls_help_set_logsize[] = {
	"\2LOGSIZE\2 <bytes>",
	"File size in bytes at which a logfile is automatically rotated",
	NULL
};

const char *ls_help_set_logtime[] = {
	"\2LOGAGE\2 <seconds> - Specify the age in seconds that we rotate the log file, regardless of log file size",
	NULL
};

const char *ls_help_set_logdir[] = {
	"\2LOGDIR\2 <path> - path for log files",
	NULL
};

const char *ls_help_set_savedir[] = {
	"\2SAVEDIR\2 <path> - path for save log files",
	NULL
};
