#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>


#define SERVER_PORT 12345
#define BUF_SIZE 4096
#define QUEUE_SIZE 10



void get_content_type(char *filename, char *content_type);





int main(int argc, char *argv[]) {

    char content_type[50];

    get_content_type("s.css", content_type);
    printf("Content-type: %s\n", content_type);



    int sockfd, connfd;

    // buffer for outgoing files
    char buffer[BUF_SIZE];

    // holds IP address of server
    struct sockaddr_in serv_addr, cli_addr; // Note: cli_addr will be filled in with address of the peer socket.
    socklen_t clilen;

    
    memset(&serv_addr, '0', sizeof(serv_addr));  // initialise server address
    memset(buffer, '0', sizeof(buffer)); // initialise send buffer


    // build address structure to bind to socket
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any IP address (socket endpoint will be bound to all the system's network interfaces)

    // Note: sin_addr.s_addr is just a 32 bit int (4 bytes)

    serv_addr.sin_port = htons(SERVER_PORT);  // listen on the default port specified


    // create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }


    // Bind the socket to the server address
    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));


    // Listen, which allocates space to queue incoming calls for the
    // case that several clients try to connect at the same time. (non-blocking call)
    listen(sockfd, QUEUE_SIZE);


    // Must initialise to contain the size (in bytes) of the structure pointed
    // to by &cli_addr. See 'man accept' for more details.
    clilen = sizeof(cli_addr);

    // Socket is now set up and bound. Wait for connection and process it.
    // Use the accept function to retrieve a connect request and convert it
    // into a connection.
    connfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen); 

    // Note: accept will block until a connect request arrives.
    // It can be modified to be non-blocking.


    // If we don't care about a client's identity, can set client address and
    // length parameters (last 2 parameters to accept) as NULL. Otherwise,
    // before calling accept, we need to set addr parameter to a buffer large
    // enough to hold the address, and set the integer pointed to by len to
    // the size of the buffer in bytes.


    // Note: the file descriptor returned by accept is a socket descriptor
    // that is connected to the client that called connect. This new socket
    // descriptor, e.g. connfd has the same socket type and address family as
    // the original socket, sockfd.


    if (connfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }



    return 0;
}



/* Parses the filename the client wants, and
 * returns the MIME type (content type) associated
 * with the file's extension.
 */
void get_content_type(char *filename, char *content_type) {

    // Get the content type associated with the file extension.
    if (strstr(filename, ".html") != NULL) {
        strcpy(content_type, "text/html");
    } else if (strstr(filename, ".jpg") != NULL) {
        strcpy(content_type, "image/jpeg");
    } else if (strstr(filename, ".css") != NULL) {
        strcpy(content_type, "text/css");
    } else if (strstr(filename, ".js") != NULL) {
        strcpy(content_type, "application/javascript");
    }
}