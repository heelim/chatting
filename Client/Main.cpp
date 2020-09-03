#include "Client.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("사용법 : %s sever_ip  port \n", argv[0]);
        exit(0);
    }
    auto client = new Client(argv[1], atoi(argv[2]));
    client->init();
    while (1) {
        client->routine();
    } 
    //NOT REACHED
}
