#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd<0) {
        perror("cannot create socket");
        return 0;
    }
    printf("created socket, fd: %d\n", fd);

    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(7692);
    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failded");
        return 0;
    }
    printf("bind complete, port number: %d\n", ntohs(myaddr.sin_port));

    exit(0);
}
