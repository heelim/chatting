#include "Server.h"

Server::Server(int port){
    listen_sock = tcp_listen(INADDR_ANY, port, 5);
}
void Server::routine(){
    FD_ZERO(&read_fds);
    FD_SET(listen_sock, &read_fds);
    for (i = 0; i < num_user; i++)
        FD_SET(clisock_list[i], &read_fds);
    maxfdp1 = getmax() + 1;	
    if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
        exitWithError("select fail");

    if (FD_ISSET(listen_sock, &read_fds)) {
        accp_sock = accept(listen_sock,	(struct sockaddr*)&cliaddr, &addrlen);
        if (accp_sock == -1) exitWithError("accept fail");
        int temp;
        if(temp = recv(accp_sock, buf, MAXLINE, 0))
        {
            buf[temp] = 0;
        }
        addClient(accp_sock, &cliaddr, buf);
    }
}
void Server::broadcast(){
    for (i = 0; i < num_user; i++) {
        if (FD_ISSET(clisock_list[i], &read_fds)) {
            num_chat++;				
            nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
            if (nbyte <= 0) {
                removeClient(i);
                continue;
            }
            buf[nbyte] = 0;
            if (strstr(buf, EXIT_STRING) != NULL) {
                removeClient(i);
                continue;
            }

            for (j = 0; j < num_user; j++)
                send(clisock_list[j], buf, nbyte, 0);
            printf("\033[0G");		
            printf("%s", buf);	
            fprintf(stderr, "server>");
        }
    }
}
void Server::announceNumClient(char* announcement){
    printf("%s, Current: %d Clients\n", announcement, num_user);
}

void Server::addClient(int s, struct sockaddr_in *newcliaddr, char* name) {
    char buf[20];
    inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
    write(1, "\033[0G", 4);		
    printf("new client: %s: %s\n", name, buf);

    clisock_list[num_user] = s;
    strcpy (cliname_list[num_user], name);
    strcpy(ip_list[num_user], buf);
    num_user++;
    ct = time(NULL);			
    tm = *localtime(&ct);
    write(1, "\033[0G", 4);	
    printf("[%02d:%02d:%02d] ", tm.tm_hour, tm.tm_min, tm.tm_sec);
    announceNumClient("One Client entered");
    fprintf(stderr, "server>");
}

void Server::removeClient(int s) {
    close(clisock_list[s]);
    if (s != num_user - 1) { 
        clisock_list[s] = clisock_list[num_user - 1];
        strcpy(ip_list[s], ip_list[num_user - 1]);
    }
    num_user--;
    ct = time(NULL);		
    tm = *localtime(&ct);
    write(1, "\033[0G", 4);	
    printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
    announceNumClient("One Client exit");
    fprintf(stderr, "server>");
}

int Server::getmax() {

    int max = listen_sock;
    int i;
    for (i = 0; i < num_user; i++)
        if (clisock_list[i] > max)
            max = clisock_list[i];
    return max;
}

int Server::tcp_listen(int host, int port, int backlog) {
    int sd;
    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        perror("socket fail");
        exit(1);
    }
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(host);
    servaddr.sin_port = htons(port);
    if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind fail");  exit(1);
    }
    listen(sd, backlog);
    return sd;
}
