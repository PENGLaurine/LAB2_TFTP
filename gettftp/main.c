#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {

    if (argc != 3){
        printf("Error: number of arguments");
        exit(EXIT_FAILURE);
    }

    char* host = argv[1];
    char* file = argv[2];

    //get address information of the server
    struct addrinfo hints;
    struct addrinfo *result,*rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 */

    int sfd, s = getaddrinfo(host,NULL, &hints, &result);

    if (s!=0){
        printf("Error: getaddrinfo failure\n");
        exit(EXIT_FAILURE);
    }

    for(rp=result; rp!=NULL; rp=rp->ai_next){
        //print ip address and address info
        char* ipverstr;
        switch (rp->ai_family){
            case AF_INET:
                ipverstr = "IPv4";
                break;
            case AF_INET6:
                ipverstr = "IPv6";
                break;
            default:
                ipverstr = "unknown";
                break;
        }
        struct sockaddr_in *addr;
        addr = (struct sockaddr_in *)rp->ai_addr;
        fprintf(stdout,"addr ip : %s ",ipverstr);
        fprintf(stdout,"%s\n",inet_ntoa((struct in_addr)addr->sin_addr));
        fprintf(stdout,"addrinfo:\n--family: %d\n--socktype: %d\n--protocol: %d\n\n",rp->ai_family,rp->ai_socktype,rp->ai_protocol);

        //reserve a connection socket to the server
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

        close(sfd);
    }

    freeaddrinfo(result);

}
