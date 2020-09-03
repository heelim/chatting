#include "../Common/Common.h"

#define MAXLINE     1000
#define NAME_LEN    20

class Client {
    public:
        Client(char*, int);
        void init();
        void routine();
    private:
        char *EXIT_STRING = "exit";
        char bufname[NAME_LEN];
        char bufmsg[MAXLINE];	
        char bufall[MAXLINE + NAME_LEN];
        char nickname[MAXLINE];
        int maxfdp1;	
        int namelen;	
        fd_set read_fds;
        time_t ct;
        struct tm tm;
        struct sockaddr_in servaddr;
        int  s;
        char* ip;
        int port;
};
