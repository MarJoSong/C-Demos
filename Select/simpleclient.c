#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    char buf[256];

    while (1) {
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        retval = select(1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
            perror("select()");
        else if (retval)
            printf("Data is available now.\n");
        else
            printf("No data within five seconds.\n");

        if (FD_ISSET(1, &rfds)) {
            bzero(buf, 256);
            read(1, buf, 256);
            printf("recv msg:[%s]\n", buf);
        }
    }

    return 0;
}

