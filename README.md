`bigmix`
===
Bash script to shuffle play specific dir and its subdirs with `mplayer`
since mplayer doesn't like to do that recursively.


configuration
===
Edit bigmix.sh and specify the folder $AUD\_BASE under which all music is
located.  This folder will also be the default if no valid location is
specifed when called from command line.

Other important variables -- see vars at top of script:
$DBFILE -- text file path to hold generated playlist.
$NCORES -- number of cores to use in randomisation


You can also modify some args to `mplayer` at the end of the script.
`-vo null` is set, for instance to disable video from appearing when a
.flv file is accessed for instance.


depends
===
`mplayer`


usage
===
./bigmix.sh [dir]
If a valid directory, `dir`, is given, then shuffles music therein.
Otherwise uses AUD\_BASE (cf. configuration).

