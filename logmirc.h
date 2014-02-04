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
** $Id: logmirc.h 124 2006-01-26 15:33:47Z Fish $
*/

#ifndef LOGMIRC_H
#define LOGMIRC_H

void mirc_startlog( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_joinproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_partproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_msgproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_noticeproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_ctcpaction( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_quitproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_topicproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_kickproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_nickproc( ChannelLog *chandata, const CmdParams *cmdparams );
void mirc_modeproc( ChannelLog *chandata, const CmdParams *cmdparams );

#endif
