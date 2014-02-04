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
** $Id: logdefault.h 124 2006-01-26 15:33:47Z Fish $
*/

#ifndef LOGDEFAULT_H
#define LOGDEFAULT_H

void logserv_startlog( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_joinproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_partproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_msgproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_noticeproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_ctcpaction( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_quitproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_topicproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_kickproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_nickproc( ChannelLog *chandata, const CmdParams *cmdparams );
void logserv_modeproc( ChannelLog *chandata, const CmdParams *cmdparams );

#endif
