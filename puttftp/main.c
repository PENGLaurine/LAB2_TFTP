#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

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
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */


    int s = getaddrinfo(argv[1],NULL, &hints, &result);

    if (s!=0){
        printf("Error: getaddrinfo failure\n");
        exit(EXIT_FAILURE);
    }

    int sfd;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            fprintf(stdout,"%s\n",rp->ai_addr->sa_data);
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"%s\n",rp->ai_addr->sa_data);

    freeaddrinfo(result);
}
