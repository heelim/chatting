#include "Client.h"
using namespace std;
Client::Client(char* Ip, int Port) {
    ip=Ip;
    port=Port;
}
void Client::init() {
    cout<<"Enter your nickname : ";
    cin >> nickname;   
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exitWithError("tcp socket init fail");
    printf("Socket initalized\n");
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        exitWithError("tcp connect fail");
    printf("Server Connected\n");
    FD_SET(0, &read_fds);
    FD_SET(s, &read_fds);
    /*    if (FD_ISSET(s, &read_fds)) {
          int nbyte;
          nbyte = recv(s, bufmsg, MAXLINE, 0);
          bufmsg[nbyte-1] = 0;
          printf("%s", bufmsg);
          }*/
    if (FD_ISSET(0, &read_fds)) {
        int length=strlen(nickname);
        if (send(s, nickname, length, 0) < 0) puts("Error : Write error on socket.");
        fprintf(stderr, "%s>", nickname);
    }
    maxfdp1 = s + 1;
    FD_ZERO(&read_fds);
}
void Client::routine() {
    FD_SET(0, &read_fds);
    FD_SET(s, &read_fds);
    if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
        exitWithError("select fail");
    ct = time(NULL);
    tm = *localtime(&ct);
    if (FD_ISSET(s, &read_fds)) {
        int nbyte;
        if ((nbyte = recv(s, bufmsg, MAXLINE, 0)) > 0) {
            bufmsg[nbyte] = 0;
            fprintf(stderr, "\033[0G");
            printf("%s", bufmsg);
            fprintf(stderr, "%s>", nickname);
        }
    }
    if (FD_ISSET(0, &read_fds)) {
        if (fgets(bufmsg, MAXLINE, stdin)) {
            if (strstr(bufmsg, "\n") == NULL) {
                fprintf(stderr, "\033[1A");
                sprintf(bufall, "[%02d:%02d:%02d] %s>%s", tm.tm_hour, tm.tm_min, tm.tm_sec, nickname, bufmsg);
                if (send(s, bufall, strlen(bufall), 0) < 0)
                    puts("Error : Write error on socket.");
                if (strstr(bufmsg, EXIT_STRING) != NULL) {
                    puts("Good bye.");
                    close(s);
                    exit(0);
                }
            }
            else {
            fprintf(stderr, "\033[0G");
            fprintf(stderr, "%s>", nickname);
            }
        }
    }
}
