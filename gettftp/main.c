#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc, char** argv) {

    if (argc != 3){
        printf("Error: number of arguments");
        exit(EXIT_FAILURE);
    }

    int sock;
    if ((sock = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    char* host = argv[1];
    char* file = argv[2];


}
