#include "../Common/Common.h"

#define MAXLINE  511
#define MAX_SOCK 1024 


class Server {
    public:
        Server(int port);
        void addClient(int s, struct sockaddr_in *newcliaddr, char* name);
        void errquit(char *mesg) { perror(mesg); exit(1); }
        void removeClient(int s);	
        void routine();
        void broadcast();
        void announceNumClient(char*);
    private:
        char *EXIT_STRING = "exit";	
        char *START_STRING = "Server:Connected to chat_server \n";
        int maxfdp1;				
        int num_user = 0;			
        int num_chat = 0;			
        int clisock_list[MAX_SOCK];		
        char cliname_list[MAX_SOCK][MAXLINE];		
        char ip_list[MAX_SOCK][20];		

        int listen_sock;    
        int getmax();				
        int tcp_listen(int host, int port, int backlog); 

        time_t ct;
        struct tm tm;

        struct sockaddr_in cliaddr;
        char buf[MAXLINE + 1];
        int i, j, nbyte, accp_sock;
        unsigned int addrlen = sizeof(struct sockaddr_in);
        fd_set read_fds;	
        pthread_t a_thread;
};
