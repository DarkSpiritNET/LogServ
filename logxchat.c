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
** $Id: logxchat.c 132 2008-02-26 05:34:57Z Fish $
*/

#include "neostats.h"
#include "logserv.h"
#include "logxchat.h"

/* XCHAT FORMAT LOGGING BELOW */

static char timebuf[TIMEBUFSIZE];
#define XCHATTIME "%b %d %H:%M:%S"

static char *xchat_time( void )
{
	os_strftime( timebuf, TIMEBUFSIZE, XCHATTIME, os_localtime( &me.now ) );
	return timebuf;
}
/* **** BEGIN LOGGING AT Fri Jan  2 17:25:15 2004
 * \n
 */
#define XSTARTLOG "**** BEGIN LOGGING AT %s\n\n"
 
void xchat_startlog( ChannelLog *chandata, const CmdParams *cmdparams )
{
	static char startlog[BUFSIZE];
	static char tmbuf[TIMEBUFSIZE];
	
	os_strftime( tmbuf, TIMEBUFSIZE, "%a %b %d %H:%M:%S %Y", os_localtime( &me.now ) );
	ircsnprintf( startlog, BUFSIZE, XSTARTLOG, tmbuf );
	os_fprintf( chandata->logfile, "%s", startlog );
}


/* Jan 02 17:27:10 -->     Dirk-Digler( fish@Dirk-Digler.Users.irc-chat.net ) has joined #neostats */
#define XJOINFMT "%s -->\t%s( %s@%s ) has joined %s\n"

void xchat_joinproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XJOINFMT, xchat_time(), cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->vhost, cmdparams->channel->name );
}

/* Jan 02 17:56:52 <--     DigiGuy( ~b.dole@Oper.irc-chat.net ) has left #neostats( part )*/
#define XPARTPROC "%s <--\t%s( %s@%s ) has left %s( %s )\n"

void xchat_partproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XPARTPROC, xchat_time(), cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->vhost, cmdparams->channel->name, cmdparams->param );
}

/* Jan 02 17:25:43 <SecureServ>    Akilling jojo!~jojo@pD9E60152.dip.t-dialin.net for Virus IRCORK */

#define XMSGFMT "%s <%s>\t%s\n"

void xchat_msgproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XMSGFMT, xchat_time(), cmdparams->source->name, cmdparams->param );
}

void xchat_noticeproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XMSGFMT, xchat_time(), cmdparams->source->name, cmdparams->param );
}

/* Action: 
 * Jan 02 17:28:52 *       Fish-Away sighs */
#define XACTFMT "%s *\t%s %s\n"

void xchat_ctcpaction( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XACTFMT, xchat_time(), cmdparams->source->name, cmdparams->param );
}

/* Jan 02 17:47:26 <--     Dirk-Digler has quit( Killed( Fish( get lost ) ) ) */
#define XQUITFMT "%s <--\t%s has quit( %s )\n"

void xchat_quitproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XQUITFMT, xchat_time(), cmdparams->source->name, cmdparams->param );
}

/* Jan 02 17:48:12 ---     Digi|Away has changed the topic to: FREE PORN - DETAILS INSIDE */
#define XTOPICPROC "%s ---\t%s has changed the topic to: %s\n"

void xchat_topicproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	if (cmdparams->source) {
		ls_write_log( chandata, XTOPICPROC, xchat_time(), cmdparams->source->name, cmdparams->param );
	} else {
		ls_write_log( chandata, XTOPICPROC, xchat_time(), cmdparams->channel->topicowner, cmdparams->param );
	}		
}

/* Jan 02 17:27:10 <-- Fish-Away has kicked Dirk-Digler from #neostats( ha ) */
#define XKICKPROC "%s <--\t%s has kicked %s from %s( %s )\n"

void xchat_kickproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XKICKPROC, xchat_time(), cmdparams->source->name, cmdparams->target->name, cmdparams->channel->name, cmdparams->param );
}

/* Jan 02 17:50:32 ---     DigiGuy is now known as Bob */
#define XNICKPROC "%s ---\t%s is now known as %s\n"

void xchat_nickproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, XNICKPROC, xchat_time(), cmdparams->param, cmdparams->source->name );
}

/* Jan 02 17:27:10 ---     SuperSexSquirrel sets modes [#NeoStats +v Dirk-Digler] */
#define XMODEPROC "%s ---\t%s sets modes[%s %s]\n"

void xchat_modeproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	char *modebuf;
	
	modebuf = joinbuf( cmdparams->av, cmdparams->ac, 0 );
	ls_write_log( chandata, XMODEPROC, xchat_time(), cmdparams->source->name, chandata->channame, modebuf );
	ns_free( modebuf );
}
