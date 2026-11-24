#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT 5000
#define MAXLINE 1000


#define BUFFER_LEN 100


// every message received ends with '\0'
int main() {
	
	// create datagram socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
    
    // connect to server
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }

    // request to send datagram
    // no need to specify server address in sendto
    // connect stores the peers IP and port
	char buffer_write[BUFFER_LEN] = { 0 };
	strcpy(buffer_write, "Hello from client");
    sendto(sockfd, buffer_write, strlen(buffer_write) + 1, 0, (struct sockaddr*) NULL, sizeof(servaddr));
    
    // waiting for response
	char buffer_read[BUFFER_LEN] = { 0 };
    while(strcmp(buffer_read, "DISCONNECT") != 0) {
		ssize_t len = recvfrom(sockfd, buffer_read, BUFFER_LEN, 0, (struct sockaddr*) NULL, NULL);
		
		printf("[CLIENT]%s\n", buffer_read);
		
		if(len == BUFFER_LEN) {
			printf("\n Error : Truncated message\n");
			return 1;
		}
	}

    close(sockfd);
}
