#include "Common.h"

void exitWithError(char *mesg) { 
    perror(mesg); 
    exit(1); 
}
