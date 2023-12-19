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
        fprintf(stderr,"Error: number of arguments\n");
        exit(EXIT_FAILURE);
    }

    char* host = argv[1];
    char* file = argv[2];

    //get address information of the server
    struct addrinfo hints;
    struct addrinfo *result,*rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 */
    hints.ai_socktype = SOCK_STREAM;    /*TCP*/
    hints.ai_protocol = IPPROTO_TCP;

    int sfd, s = getaddrinfo(host,NULL, &hints, &result);

    if (s!=0){
        fprintf(stderr,"Error: getaddrinfo failure\n");
        exit(EXIT_FAILURE);
    }

    for(rp=result; rp!=NULL; rp=rp->ai_next) {
        //print ip address and address info
        char *ipverstr;
        switch (rp->ai_family) {
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
        addr = (struct sockaddr_in *) rp->ai_addr;
        addr->sin_family = rp->ai_family;
        addr->sin_port = htons(1069);
        fprintf(stdout, "addr ip : %s ", ipverstr);
        fprintf(stdout, "%s\n", inet_ntoa((struct in_addr) addr->sin_addr));
        fprintf(stdout, "addrinfo:\n--family: %d\n--socktype: %d\n--protocol: %d\n\n", rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);

        //reserve a connection socket to the server
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;          /* success */

        close(sfd);
    }

    freeaddrinfo(result);

    /* write and send read request to the server */
    char message[1024];
    message[0]='\0';
    message[1]='\1';
    int i;
    for (i=0;i<sizeof file;i++){
        message[i+2] = file[i];
    }
    int i2;
    for (i2=0;i2<sizeof file;i2++){
        message[i+i2+2] = "octet"[i2];
    }

    write(sfd, message, strlen(message));

    /* receive  reply from server*/
    char reply[1024];
    int n;

    if( (n = recv(sfd,reply,sizeof reply - 1,0)) < 0 ){
        fprintf(stderr,"Error: read\n");
        exit(EXIT_FAILURE);
    }
    reply[n] = '\0';

    /* print on the terminal */
    write(1,reply,sizeof reply);

}
