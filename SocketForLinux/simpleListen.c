//
// Created by Mars on 2020/11/9.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <strings.h>
#include <pthread.h>

#define MAXLINE 1024

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr;
    socklen_t servaddr_len;
    int listenfd;
    int flag, len = sizeof(int);

    listenfd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr_len = sizeof(servaddr);
    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(7692);

    if (-1== setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, len)) {
        printf("set port reuseable error.\n");
        exit(1);
    }

    bind(listenfd, (struct sockaddr *)&servaddr, servaddr_len);

    listen(listenfd, 20);

    //初始化及socklen_t长度赋初值是必须的，否则客户端信息取不到
    int cliaddr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in cliaddr;
    bzero(&cliaddr, cliaddr_len);

    int connectfd = accept(listenfd, &cliaddr, &cliaddr_len);

    int n;
    char buf[MAXLINE], ip[16];
    while (n = read(connectfd, buf, MAXLINE)) {

        if (n<0) {
            printf("Connection with [%s] Error.\n",
                   inet_ntop(PF_INET, &cliaddr.sin_addr, ip, MAXLINE));
            break;
        }

        printf("Read [%d] bytes from host:[%s] on port:[%d]\n", n,
               inet_ntop(PF_INET, &cliaddr.sin_addr, ip, MAXLINE),
               ntohs(cliaddr.sin_port));
        for (int i = 0; i < n; i++) {
            buf[i] = (char)toupper(buf[i]);
        }
        write(connectfd, buf, n);
    }

    return 0;
}