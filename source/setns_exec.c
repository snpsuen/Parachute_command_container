#define _GNU_SOURCE
#include <err.h>
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/mount.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

#define MINLEN 64
#define MEDLEN 128
#define MAXLEN 256
#define BUFLEN 1024

int set_ns(int cpid);

int set_ns(int cpid) {
        int pfd, nsflag;

        if ((pfd = syscall(SYS_pidfd_open, cpid, 0)) == -1)
                err(EXIT_FAILURE, "pidfd_open %d", cpid);

        if (unshare(CLONE_NEWNS) == -1)
                err(EXIT_FAILURE, "unshare CLONE_NEWNS");

        nsflag = CLONE_NEWCGROUP | CLONE_NEWIPC | CLONE_NEWNS | CLONE_NEWTIME | CLONE_NEWUTS ;
        if (setns(pfd, nsflag) == -1)
                err(EXIT_FAILURE, "setns %d %d", pfd, nsflag);

        close(pfd);
        return 0;
}

int main(int argc, char* argv[]) {
        char** execargv;

        if (argc < 2) {
                execargv = calloc(2, sizeof(char*));
                execargv[0] = "sh";
                execargv[1] = NULL;
        }
        else {
                execargv = calloc(argc, sizeof(char*));
                for (i = 0; i < argc - 1; i++)
                        execargv[i] = argv[i + 1];
                execargv[argc - 1] = NULL;
        }

        set_ns(1);
        if (execvp(execargv[0], execargv) < 0) {
                perror("execvp error");
    
        exit(1);
}
