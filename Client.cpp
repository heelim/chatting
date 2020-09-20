#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: %s port", argv[0]);
        exit(1);
    }
    int port = atoi(argv[1]);
    int client_socket;
    struct sockaddr_in server_addr;
    char buf[BUF_SIZE];
    
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(client_socket == -1)
    {
        perror("socket 생성 실패\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connection failed\n");
        exit(1);
    }
    fgets(buf, sizeof(buf), stdin);
    write(client_socket, buf, strlen(buf)+1); // +1: NULL까지 포함해서 전송
    read(client_socket, buf, BUF_SIZE);
    printf("%s\n", buf);
    close(client_socket);

    return 0;
}
