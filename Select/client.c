#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXLINE 1024

static void handle_connection(int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    int maxfdp, stdineof;
    fd_set readfds;


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

    printf("client send to server.\n");
    write(sockfd, "hello server", 32);

    handle_connection(sockfd);

    return 0;
}

