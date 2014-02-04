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
** $Id: logserv.h 124 2006-01-26 15:33:47Z Fish $
*/

#ifndef LOGSERV_H
#define LOGSERV_H

/* Channel Logging Struct. */
typedef struct ChannelLog {
	char channame[MAXCHANLEN];
	Channel *c;
	FILE *logfile;
	char filename[MAXPATH];
	unsigned long flags;
	char statsurl[MAXPATH];
	time_t ts_open;
	int writecount;
} ChannelLog;


/* settings for LogServ */
typedef struct LogServcfg {
	int logtype;
	char logdir[MAXPATH];
	char savedir[MAXPATH];
	long maxlogsize;
	long maxopentime;
} LogServcfg;

extern LogServcfg LogServ;

/* Definitions for flags */
#define LGSPUBSTATS 0x1
#define LGSACTIVE 0x2

/* the hash that stores the monitored channels */
extern hash_t *lschannelhash;

typedef enum {
	LGSMSG_START = 0,
	LGSMSG_JOIN,
	LGSMSG_PART,
	LGSMSG_MSG,
	LGSMSG_NOTICE,
	LGSMSG_CTCPACTION,
	LGSMSG_QUIT,
	LGSMSG_TOPIC,
	LGSMSG_KICK,
	LGSMSG_NICK,
	LGSMSG_CHANMODE,
	LGSMSG_NUMTYPES
} LGSMSG_TYPE;
	

/* logging function prototype */
typedef void (*log_proc) ( ChannelLog *chandata, const CmdParams *cmdparams );

/* log_procssing.c decl */
void ls_send_to_logproc ( LGSMSG_TYPE msgtype, const Channel *c, const CmdParams *cmdparams );
int ls_rotate_logs( void * );
void ls_close_logs( void );
void ls_switch_file( ChannelLog *cl );
void ls_write_log( ChannelLog *cl, const char *fmt, ...) __attribute__( ( format( printf,2,3 ) ) );

extern const char *ls_about[];
extern const char *ls_help_add[];
extern const char *ls_help_del[];
extern const char *ls_help_list[];
extern const char *ls_help_url[];
extern const char *ls_help_status[];
extern const char *ls_help_set_logtype[];
extern const char *ls_help_set_logsize[];
extern const char *ls_help_set_logtime[];
extern const char *ls_help_set_logdir[];
extern const char *ls_help_set_savedir[];

#endif
