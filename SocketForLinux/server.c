#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <strings.h>
#include <pthread.h>

#define MAXLINE 1024
#define NUM_THREADS 20

typedef struct __st_deal_conn {
    int *connectfd;
    struct sockaddr_in *cliaddr;
} st_deal_conn;

// 线程的运行函数
void* deal_connection(void* args)
{
    int n;
    char ip[MAXLINE];
    char buf[MAXLINE];
    st_deal_conn *deal_conn_st = (st_deal_conn *)args;

    while (1) {
        n = read(*deal_conn_st->connectfd, buf, MAXLINE);
        /*
         * char *inet_ntoa(struct in_addr in);
         * 将Internet主机地址(网络字节序)转换为IPv4点状十进制符号的字符串。
         * 该字符串在静态分配的缓冲区中返回，后续调用将覆盖该缓冲区。
         *
         * const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
         * 支持ipv6，转换结果保存在src
         */
        if (n<=0) {
            printf("Host [%s] lost connection.\n",
                   inet_ntop(PF_INET, &deal_conn_st->cliaddr->sin_addr, ip, MAXLINE));
            break;
        }
        printf("Read len [%d] bytes from host:[%s] on port:[%d]\n", n,
               inet_ntop(PF_INET, &deal_conn_st->cliaddr->sin_addr, ip, MAXLINE),
               ntohs(deal_conn_st->cliaddr->sin_port));
        for (int i = 0; i < n; i++) {
            buf[i] = toupper(buf[i]);
        }
        write(*deal_conn_st->connectfd, buf, n);
    }

    return NULL;
}

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

    int i=0;
    pthread_t deal_threads[NUM_THREADS];
    st_deal_conn *deal_conn_st;
    printf("Host:[%s] accepting connections on port:[%d]\n",
           inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    while (i<NUM_THREADS) {
        int *connectfd = (int *)malloc(sizeof(int));
        socklen_t *cliaddr_len = (socklen_t *)malloc(sizeof(socklen_t));
        struct sockaddr_in *cliaddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));

        *connectfd = accept(listenfd, (struct sockaddr *)cliaddr, cliaddr_len);
        printf("accpet from host:[%s] on port:[%d]\n",
               inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port));
        deal_conn_st = (st_deal_conn *)malloc(sizeof(st_deal_conn));
        deal_conn_st->connectfd = connectfd;
        deal_conn_st->cliaddr = cliaddr;
        printf("accpet from host:[%s] on port:[%d]\n",
               inet_ntoa(deal_conn_st->cliaddr->sin_addr),
               ntohs(deal_conn_st->cliaddr->sin_port));
        if (pthread_create(&deal_threads[i++], NULL, deal_connection, deal_conn_st)) {
            printf("create deal thread failed.\n");

            close(listenfd);
            exit(1);
        } else {
            printf("Host:[%s] request connection on port:[%d].\n",
                   inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port));
        }
    }

    return 0;
}