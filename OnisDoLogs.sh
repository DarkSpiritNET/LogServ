#!/bin/sh
LANG=C
# Channel log Directory. Where Logfiles are stored. Only need to change this
#if you installed neostats into a different prefix other than NeoStats
chanlogs="$HOME/NeoStats3.0/ChanLogs/"

# Onis Installation Path
onisinstall="$HOME/onis-0.4.9/" 

# Output Path for Stats. Has to be writtable by the user executing this script
onisoutput="/var/www/html/stats/"

# optional Onis Commandline options
onisoptions=""

#=============================================================================
# End of Configuration. Dont edit anything below this line
#=============================================================================

LANG=C


doonis() {
	#test if output directory exists 
	if [ ! -d $chanlogs/$1/html ]; then
		mkdir $chanlogs/$1/html
	fi
	cd $chanlogs/$1
	files=`ls $chanlogs/$1/*.log`
	command="$onisinstall/onis --config $chanlogs/onisconfig --output $onisoutput/$1.html --channel \"$1\" --persistency_file $chanlogs/$1/persistancy.data --overwrite true $onisoptions $files"
	echo `$command`
	cd $chanlogs
}

cd $chanlogs


directorys=`ls $chanlogs`
for channel in $directorys; do
    if [ -d $channel ]
	then doonis $channel
    fi
done
