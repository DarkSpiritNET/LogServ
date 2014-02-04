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
** $Id: logeggdrop.c 132 2008-02-26 05:34:57Z Fish $
*/

#include "neostats.h"
#include "logserv.h"
#include "logeggdrop.h"

static char timebuf[TIMEBUFSIZE];
#define EGGTIME "[%H:%M]"

static char *egg_time( void )
{
	os_strftime( timebuf, TIMEBUFSIZE, EGGTIME, os_localtime( &me.now ) );
	return timebuf;
}

/* [00:00] --- Fri Jan  2 2004 */
/* this usually goes at the end of a logfile. Hrm */

void egg_startlog( ChannelLog *chandata, const CmdParams *cmdparams )
{
	static char startlog[BUFSIZE];

	os_strftime( startlog, BUFSIZE, "[%H:%M] --- %a %b %d %Y\n", os_localtime( &me.now ) );
	os_fprintf( chandata->logfile, "%s", startlog );
}

/* [22:02] Fish( ~Fish@Server-Admin.irc-chat.net ) joined #neostats. */
#define EJOINPROC "%s %s( %s@%s ) joined %s.\n"

void egg_joinproc( ChannelLog *chandata, const CmdParams *cmdparams ) 
{
	ls_write_log( chandata, EJOINPROC, egg_time(), cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->vhost, cmdparams->channel->name );
}

/* [22:02] Fish( ~Fish@Server-Admin.irc-chat.net ) left #neostats( ha ). */
#define EPARTPROC "%s %s( %s@%s ) left %s( %s ).\n"

void egg_partproc( ChannelLog *chandata, const CmdParams *cmdparams ) 
{
	ls_write_log( chandata, EPARTPROC, egg_time(), cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->vhost, cmdparams->channel->name, cmdparams->param );
}

/* [22:04] <Fish> I think that is it */
#define EMSGPROC "%s <%s> %s\n"

void egg_msgproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, EMSGPROC, egg_time(), cmdparams->source->name, cmdparams->param );
}

void egg_noticeproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, EMSGPROC, egg_time(), cmdparams->source->name, cmdparams->param );
}

/* [22:04] Action: Fish waits for the eggdrop to flush its logs now */
#define EACTPROC "%s Action: %s %s\n"

void egg_ctcpaction( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, EACTPROC, egg_time(), cmdparams->source->name, cmdparams->param );
}

/* [22:03] Fishwaldo( ~Fish@3cbc6b2b.22d996b6.singnet.com.sg ) left irc: Client closed */
#define EQUITPROC "%s %s( %s@%s ) left irc: %s\n"

void egg_quitproc( ChannelLog *chandata, const CmdParams *cmdparams ) 
{
	ls_write_log( chandata, EQUITPROC, egg_time(), cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->vhost, cmdparams->param );	
}

/* [22:02] Topic changed on #neostats by Fish!~Fish@Server-Admin.irc-chat.net: <LuShes> I'mmmm back!!!! [NeoStats Support] http://www.neostats.net */
#define ETOPICPROC "%s Topic changed on %s by %s!%s@%s: %s\n"
#define ENOUSERTOPICPROC "%s Topic changed on %s by %s: %s\n"

void egg_topicproc( ChannelLog *chandata, const CmdParams *cmdparams ) 
{
	if( cmdparams->source)
		ls_write_log( chandata, ETOPICPROC, egg_time(), cmdparams->channel->name, cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->vhost, cmdparams->param );
	else 
		ls_write_log( chandata, ENOUSERTOPICPROC, egg_time(), cmdparams->channel->name, cmdparams->channel->topicowner, cmdparams->param );
}

/* [22:02] Fish kicked from #neostats by Fish: Fish */
#define EKICKPROC "%s %s kicked from %s by %s: %s\n"

void egg_kickproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, EKICKPROC, egg_time(), cmdparams->target->name, cmdparams->channel->name, cmdparams->source->name, cmdparams->param ); 
}

/* [22:02] #NeoStats: mode change '+oa Fish Fish' by SuperSexSquirrel!super@supersexsquirrel.org */
#define EMODEPROC "%s %s: mode change '%s' by %s!%s@%s\n"
#define ENOUSERMODEPROC "%s %s: mode change '%s' by %s\n"
void egg_modeproc( ChannelLog *chandata, const CmdParams *cmdparams ) 
{
	char *modebuf;
	
	modebuf = joinbuf( cmdparams->av, cmdparams->ac, 0 );
	if( cmdparams->source->user )
	{
		ls_write_log( chandata, EMODEPROC, egg_time(), cmdparams->channel->name, modebuf, cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->vhost );
	}
	else
	{
		ls_write_log( chandata, ENOUSERMODEPROC, egg_time(), cmdparams->channel->name, modebuf, cmdparams->source->name );
	}	
}

/* [22:02] Nick change: Fish -> haha */
#define ENICKPROC "%s Nick change: %s -> %s\n"

void egg_nickproc( ChannelLog *chandata, const CmdParams *cmdparams )
{
	ls_write_log( chandata, ENICKPROC, egg_time(), cmdparams->param, cmdparams->source->name );
}

