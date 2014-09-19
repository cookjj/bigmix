`bigmix'
===
Shuffle play specific dir and its subdirs with `mplayer' since mplayer
doesn't like to do that recursively.


configuration
===
Edit bigmix.sh and specify the folder under which all music is located.
This folder will also be the default if no valid location is specifed
when called from command line.


depends
===
`mplayer`


usage
===
./bigmix.sh [dir]
If a valid directory, `dir`, is given, then shuffles music therein.
Otherwise uses AUD\_BASE (cf. configuration).

