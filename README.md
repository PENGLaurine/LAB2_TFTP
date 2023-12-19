### 2nd Year – Computer Science Major Practical Work
# LAB2_TFTP : TFTP Client

The objectif of this project is to develop a TFTP client using RFC specifications and Wireshark captures.

***
## question1
Use command-line arguments for the gettftp and puttftp programs to obtain request information (server and file)

We have just to use the arguments `argv[1]` as the server name and `argv[2]` as the file name.
We can also return an error if the number of arguments is not correct: `argc != 3`

***
## question2
Call getaddrinfo to obtain the server’s address.

    struct addrinfo hints;
    struct addrinfo *result,*rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 */
    hints.ai_socktype = SOCK_STREAM;    /*TCP*/
    hints.ai_protocol = IPPROTO_TCP;

We have to define a structure addrinfo hints to filter sockets.

The call `int s = getaddrinfo(host, NULL, &hints, &result);` will save the result in the pointer `&result`.

If `s==0` we can get the server's address in `result->ai_addr`. (failure if not)
To print this address, we have to call `inet_ntoa` in order to get the address in a printable string format.

***
## question3
Reserve a connection socket to the server.

Firstly, we have to create an endpoint for communication (socket).

    int sfd;
    sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);

Then we can connect to the server.

    connect(sfd, rp->ai_addr, rp->ai_addrlen);

***
## question4
For gettftp:

 - Build a properly formed Read Request (RRQ) and send it to the server:

   `write(sfd, message, strlen(message));` where message is the request.

 - Receive a file:
We can receive by the call `recv(sfd,reply,sizeof reply - 1,0)` or `read(sfd,reply,sizeof reply -1)`

       char reply[1024];
       int n;
       if( (n = recv(sfd,reply,sizeof reply - 1,0)) < 0 ){
           fprintf(stderr,"Error: read\n");
           exit(EXIT_FAILURE);
       }
       reply[n] = '\0';

The reply message have to terminate with `\0` so we have to add this at the end.
