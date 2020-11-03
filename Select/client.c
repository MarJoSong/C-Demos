#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXLINE 1024

static void handle_recv_msg(int sockfd, char *buf)
{
    fprintf(stdout, "client recv msg is:%s\n", buf);
    sleep(5);
    write(sockfd, buf, strlen(buf) + 1);
}

static void handle_connection(int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    int maxfdp, stdineof;
    fd_set readfds;
    int n;
    struct timeval tv;
    int retval = 0;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        maxfdp = sockfd;

        tv.tv_sec = 5;
        tv.tv_usec = 0;
        retval = select(maxfdp+1, &readfds, NULL, NULL, &tv);

        if (retval==-1) {
            return;
        }

        if (retval==0) {
            printf("client timeout.\n");
            continue;
        }

        if (FD_ISSET(sockfd, &readfds)) {
            n = read(sockfd, recvline, MAXLINE);
            if (n<=0) {
                fprintf(stderr, "client: server is close.\n");
                close(sockfd);
                FD_CLR(sockfd, &readfds);
                return;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd;
    int retval = 0;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(7692);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    retval = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (retval < 0) {
        fprintf(stderr, "connect [%s] failed, error msg:[%s]\n",
                inet_ntoa(servaddr.sin_addr), strerror(errno));
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "connect [%s] success, send hello msg\n",
            inet_ntoa(servaddr.sin_addr));
    write(sockfd, "hello server\n", strlen("hello server\n"));

    handle_connection(sockfd);

    return 0;
}

