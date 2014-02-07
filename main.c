#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* Base dir for all music */
#define AUD_BASE "/aud"

extern char **environ;
void unix_error(const char *);
void free_files(char **, int, int);
void sigint_handler(int);

char *get_aud_base(void);


void unix_error(const char *msg)
{
    int err = errno;
    fprintf(stderr, "%s: %s\n", strerror(err), msg);
    exit(-1);
}


/* free remaining strings i thru n in string array */
void
free_files(char **ptr, int i, int n)
{
    for( ; i < n; i++) {
        free(ptr[i]);
    }
    free(ptr);
    exit(1);
}


void
sigint_handler(int sig)
{   kill(0, SIGKILL);
    return;
}

/* TODO: make config file instead of macro */
char *
get_aud_base(void)
{
    return AUD_BASE;
}


int
main(int argc, char **argv)
{
    char buf[256];
    char *dir = get_aud_base();

    if(argc > 1 && argv[1]) {
        dir = argv[1];
        if(chdir(dir)) {
            printf("%s: Given audio directory doesn't exist.\n", dir);
            exit(-1);
        }
    }

    chdir(dir); /* Restrict to files under audio base directory */
    if(!strstr(getcwd(buf, 256), "aud")) {
        printf("%s: Given directory not in /aud\n", buf);
        exit(-1);
    }

    /* get formatted paths in ~/aud/, randomize them, and put to db file */
    system("find ./ -maxdepth 6 -type f -print | sort --parallel=4 -R > /tmp/zzz_db");

    char **files = NULL; /* array of filenames */
    int i, n = 0;       /* index into, and number of files */

    const char *path = "/tmp/zzz_db";
    FILE *db = fopen(path, "r");
    if(!db) { unix_error(path); }

    /* read filename lines from db text file */
    while(fgets(buf, 256, db)) {
        files = realloc(files, sizeof(char*) * (n+1));
        int len = strlen(buf);
        files[n] = calloc(len, sizeof(char));
        strncpy(files[n], buf, len-1); /* truncate newline */
        n++;
    }
    fclose(db);

    /* install handler to pass a SIGKILL upon receiving C-v */
    signal(SIGINT, sigint_handler);

    /* iterate through list, fork off to play file, free assoc memory */
    for(i = 0; i < n; i+=4) {
        pid_t pid = fork();
        if(pid < 0) exit(-1);

        /* infans */
        if(pid == 0) {
            execlp("mplayer", " -novideo", /* 4 files per call */
                   files[i], files[i+1], files[i+2], files[i+3],
            (char*)NULL);
            unix_error("couldn't play song");
            exit(-1);
        }

        /* Rest of loop and main is Parent way of fork */

        int status;
        while(!waitpid(pid, &status, WUNTRACED)) ;

        /* if child returned b/c of signal (SIGKILL), then end our program */
        if(WIFSIGNALED(status)) {
            /* no return from this call,
               but rather exit() after freeing memory */
            free_files(files, i, n);
        }

        free(files[i]);
        free(files[i+1]);
        free(files[i+2]);
        free(files[i+3]);
    }

    free(files);
    return 0;
}

