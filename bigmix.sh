#!/bin/bash
AUD_BASE=/aud
AUD=$AUD_BASE
DBFILE=/bpool/stuff/tmp/zzz_db
NCORES=4

if [[ ! -d "$AUD_BASE" ]];
then
    echo "AUD_BASE=$AUD_BASE is not valid dir!"
    exit 1
fi

# Behaviour definition:
# Directory arg given?
# Yes:
#       Is it under AUD_BASE?
#       Yes:
#           cd to it, check sucess, check it's under Base dir
#       No:
#           cd AUD_BASE
# No:
#       cd AUD_BASE

# Arg given?
if [[ $# -eq 1 ]];
then
    cd $1 #goto it
    if [[ $? -eq 0 ]]; #if it worked
    then
        AUD=`pwd` #then use pwd as aud dir

        # ensure that we're actually under AUD_BASE somewhere
        audbaselen=`expr length $AUD_BASE`
        pwdstr=${AUD:0:$audbaselen}
        if [ "$pwdstr" != "$AUD_BASE" ]
        then
#            echo "Supplied subdir not in AUD_BASE -- using $AUD_BASE."
            AUD=$AUD_BASE
        fi
    else
        AUD=$AUD_BASE
    fi
fi
cd $AUD

# rm the db file list if it's been used allready
rm $DBFILE
find -maxdepth 7 -type f -printf "$AUD/%p\n" | sort --parallel=$NCORES -R > $DBFILE
mplayer -vo null -playlist $DBFILE












