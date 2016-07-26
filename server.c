//server.c: LISTEN/TALK
//command line flow: compile and run both client (include localhost as argv[1]) and server (specify -o flags because both can't have a.out)
//type in username for each (server will be in waiting state until you type in message on client)

//std libraries placed above network/socket libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

//server main doesn't need optional parameter arguments 
int main(){

//vars needed for program 
char message[200];
char username[20]; //username
char commWith[20]; //username of person you're communicating with
char incomMessage[200]; //message of person you're comm with 

//assigns nost name and opens socket; clears preexisting data  
struct sockaddr_in si_server;
bzero((char*)&si_server, sizeof(si_server));

//specifies protocol family that tells user what kind of addresses our socket can comm with
si_server.sin_family = AF_INET;
si_server.sin_addr.s_addr=INADDR_ANY;

//hardcoded port number
si_server.sin_port=htons(58619);

//specifications on kind of socket we want
int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
bind(sockfd, (struct sockaddr*)&si_server, sizeof(si_server)); //only one of the files (ppl in the chat) needs to bind 

//promts user for username to chat by
printf("Enter username (no more than 20 characters long): "); //my username (not of person I'm talking to)
fgets(username, sizeof(username), stdin); 

printf("\n"); 

socklen_t len = sizeof(si_server); 

while(1){

//receives username from person you're comm with (recvfrom before sendto in server.c and sendto then recvfrom in client.c
recvfrom(sockfd, commWith, sizeof(commWith), 0, (struct sockaddr*)&si_server, &len); 

//int nameLength = sizeof(commWith);  //gets rid of automatic new line character that'll come with username
//strcpy(commWith[nameLength-1], "\0");

//receives message from person you're comm with
recvfrom(sockfd, incomMessage, sizeof(incomMessage), 0, (struct sockaddr*)&si_server, &len);

//check for "quit" on friend's side (assume quit is spelled in all lowercase)
if(strcmp(incomMessage, "quit\n") == 0){
printf("Nice chatting with you!\n"); 
break;
}


//prints out chat partner's name and message 
printf("%s says: ", commWith); 
printf("%s\n", incomMessage);

//promts user for message to send
printf("Type message you'd like to send (no more than 200 characters long): ");
fgets(message, sizeof(message), stdin);

printf("\n"); 

//sends username to server(localhost here)
sendto(sockfd, username, sizeof(username), 0, (struct sockaddr*)&si_server, sizeof(si_server)); 

//sends message to server
sendto(sockfd, message, sizeof(message), 0, (struct sockaddr*)&si_server, sizeof(si_server)); 

//check for "quit" on my side
if(strcmp(message, "quit\n") ==0){
printf("Nice chatting with you!\n");
break;
}

}//end while loop 
return 0;           
}
