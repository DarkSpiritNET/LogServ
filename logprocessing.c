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
** $Id: logprocessing.c 124 2006-01-26 15:33:47Z Fish $
*/

#include "neostats.h"
#include "logserv.h"
#include "logdefault.h"
#include "logmirc.h"
#include "logxchat.h"
#include "logeggdrop.h"

/* only check logfile size every X calls */
#ifdef DEBUG
#define DOSIZE 1
#else 
#define DOSIZE 50
#endif

static log_proc logging_funcs[][LGSMSG_NUMTYPES] =
{
	{logserv_startlog, logserv_joinproc, logserv_partproc, logserv_msgproc, logserv_noticeproc, logserv_ctcpaction, logserv_quitproc, logserv_topicproc, logserv_kickproc, logserv_nickproc, logserv_modeproc},
	{egg_startlog, egg_joinproc, egg_partproc, egg_msgproc, egg_noticeproc, egg_ctcpaction, egg_quitproc, egg_topicproc, egg_kickproc, egg_nickproc, egg_modeproc},
	{mirc_startlog, mirc_joinproc, mirc_partproc, mirc_msgproc, mirc_noticeproc, mirc_ctcpaction, mirc_quitproc, mirc_topicproc, mirc_kickproc, mirc_nickproc, mirc_modeproc},
	{xchat_startlog, xchat_joinproc, xchat_partproc, xchat_msgproc, xchat_noticeproc, xchat_ctcpaction, xchat_quitproc, xchat_topicproc, xchat_kickproc, xchat_nickproc, xchat_modeproc},
};

/* @brief Opens the log file, creating directories where necessary
 *
 * @param cl the channel log function
 *
 * @returns a boolen indicating success/failure
 */ 

static int ls_open_log( ChannelLog *cl )
{
	static char fname[MAXPATH];

	/* first, make sure the logdir dir exists */
	if( os_create_dir( LogServ.logdir ) != NS_SUCCESS )
	{
		return NS_FAILURE;
	}
	/* copy name to the filename holder( in case of invalid paths ) */
	strlcpy( cl->filename, cl->channame, MAXPATH );
	ircsnprintf( fname, MAXPATH, "%s/%s.log", LogServ.logdir, make_safe_filename( cl->filename ) );
	/* open the file */
	cl->logfile = os_fopen( fname, "a" );
	if( cl->logfile == NULL )
	{
		nlog( LOG_CRITICAL, "Could not open %s for appending: %s", cl->filename, os_strerror() );
		return NS_FAILURE;
	}
	dlog( DEBUG1, "Opened %s for appending", cl->filename );
	cl->ts_open = me.now;
	logging_funcs[LogServ.logtype][LGSMSG_START]( cl, NULL );
	return NS_SUCCESS;
}

/* @brief Close the log file
 *
 * @param cl the channel log function
 *
 * @return none
 */ 

static void ls_close_log( ChannelLog *cl )
{
	if( cl->logfile )
		os_fclose( cl->logfile );
	cl->logfile = NULL;
	cl->ts_open = 0;
}

/* @brief check the logfile size, and rotate if necessary
 * 
 * @param cl the ChannelLog struct for the channel we are checking 
 */ 
static void ls_stat_file( ChannelLog *cl ) 
{
	static char fname[MAXPATH];
	int filesize = 0;
	
	/* reset this counter */
	cl->writecount = 0;
	/* construct the filename to stat */
	ircsnprintf( fname, MAXPATH, "%s/%s.log", LogServ.logdir, cl->filename );
	filesize = os_file_get_size( fname );
	if( filesize <= 0 ) {
		return;
	}
	dlog( DEBUG1, "Logfile Size of %s is %d", fname, filesize );
	if( filesize > LogServ.maxlogsize ) {
		dlog( DEBUG1, "Switching Logfile %s", fname );
		/* ok, the file exceeds out limits, lets switch it */
		ls_switch_file( cl );
	}
}


/* @brief write a formatted log to the log files, and check if we should switch th logfile
 * 
 * writes a message to a log file. If the logfile hasn't been opened yet, we 
 * open it, and write out any headers required.
 * 
 * @params cl The ChannelLog structreu for the channel we are loggin
 * @params fmt the printf style format of the log message
 */


void ls_write_log( ChannelLog *cl, const char *fmt, ... )
{
	static char log_buf[BUFSIZE];
	va_list ap;
	
	/* format the string to write */
	va_start( ap, fmt );
	ircvsnprintf( log_buf, BUFSIZE, fmt, ap );
	va_end( ap );
	                                
	/* if the FD isn't opened yet, lets open a log file */
	if( cl->logfile == NULL  ) {
		if( ls_open_log( cl ) != NS_SUCCESS ) {
			return;
		}
	}
	/* ok, file is opened. write the string to it */
#ifdef DEBUG
	dlog( DEBUG1, "%s\n", log_buf );
#endif
	os_fprintf( cl->logfile, "%s", log_buf );
	cl->writecount++;
#ifdef DEBUG
	/* only flush the logfile in debug mode */
	os_fflush( cl->logfile );
#endif
	/* ok, now stat the file to check size */
	if( cl->writecount >= DOSIZE ) { 
		os_fflush( cl->logfile );
		ls_stat_file( cl );
	}
}

/* @brief actually switch the logfile, saving the old log in a different directory
 *
 * @param cl the ChannelLog struct 
 */

void ls_switch_file( ChannelLog *cl )
{
	static char tmbuf[MAXPATH];
	static char newfname[MAXPATH];
	static char oldfname[MAXPATH];
	int res;

	/* no need to switch, its not opened */
	if( cl->logfile == NULL  ) return;		
	ls_close_log( cl );
	/* check if the target directory exists */
	if( os_create_dir( LogServ.savedir ) != NS_SUCCESS )
	{
		return;
	}
	os_strftime( tmbuf, MAXPATH, "%d%m%Y%H%M%S", os_localtime( &me.now ) );
	ircsnprintf( newfname, MAXPATH, "%s/%s-%s.log", LogServ.savedir, cl->filename, tmbuf );
	ircsnprintf( oldfname, MAXPATH, "%s/%s.log", LogServ.logdir, cl->filename );
	res = os_rename( oldfname, newfname );
	if( res != 0 ) {
		nlog( LOG_CRITICAL, "Couldn't rename file %s: %s", oldfname, os_strerror() );
		return;
	}	
	nlog( LOG_NORMAL, "Switched Logfile for %s from %s to %s", cl->channame, oldfname, newfname );
}
/* @brief Close all logfiles and delete the struct assocated with them
 *
 * Called from ModFini when we are unloaded, to cleanup
 */

void ls_close_logs( void )
{
	hscan_t hs;
	hnode_t *hn;
	ChannelLog *cl;
	
	/* scan through the log files */
	hash_scan_begin( &hs, lschannelhash );
	while( ( hn = hash_scan_next( &hs ) ) != NULL )
	{
		cl = hnode_get( hn );
		ls_close_log( cl );
		if( cl->c )
		{
			ClearChannelModValue( cl->c );
			cl->c = NULL;
		}
		/* delete from the hash */
		hash_scan_delete_destroy_node( lschannelhash, hn );
		ns_free( cl );
	}
}

/* @brief Rotate the log files if they have been opened longer than the pre-defined time
 * 
 * Runs through all active opened logfiles only
 */
int ls_rotate_logs( void *userptr ) 
{
	hscan_t hs;
	hnode_t *hn;
	ChannelLog *cl;
	
	/* if Logage is 0, just bail out */
	if( LogServ.maxopentime <= 0 ) {
		return NS_SUCCESS;
	}

	/* scan through the log files */
	hash_scan_begin( &hs, lschannelhash );
	while( ( hn = hash_scan_next( &hs ) ) != NULL ) {
		cl = hnode_get( hn );
		/* if the log has been opened more than X, then rotate */
		if( ( cl->logfile ) &&( ( me.now - cl->ts_open ) > LogServ.maxopentime ) ) {
			ls_switch_file( cl );
		}
	}
	return NS_SUCCESS;
}

/* @brief Send the Log Message to the relevent Log Processor 
 *
 * @param msgtype the type of message( join, part etc )
 * @param av contents of the message 
 * @param ac message size 
 * @returns NS_SUCCESS or NS_FAILURE
 */
void ls_send_to_logproc( LGSMSG_TYPE msgtype, const Channel *c, const CmdParams *cmdparams ) 
{
	ChannelLog *cl;

	if( c )
	{
		cl =( ChannelLog * )GetChannelModValue( c );
		if( cl )
		{
			logging_funcs[LogServ.logtype][msgtype]( cl, cmdparams );
		}
	}
}
