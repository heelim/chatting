#include "Server.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("사용법 :%s port\n", argv[0]);
        exit(0);
    }
    auto server = new Server(atoi(argv[1]));
    while (1) {
        server->routine();
        server->broadcast();
    }
    //unreached
    return 0;
}
