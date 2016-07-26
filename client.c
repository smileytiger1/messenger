//client.c: TALK/LISTEN

//must place std lib files above socket/network library
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

//optional main parameters to specify hostname (parameter 1: localhost) and filename (parameter 2: output.txt)
//file server writes to can't be called same as input.txt file (sent from client) because we are ccmmunicating across our single computer (hence localhost)
int main(int argc, char*argv[]){

//vars needed for program
char username[20]; //username cannot be more than 20 chars long
char message[200];
int i = 0;
char commWith[20]; //username of person you're communicating with
char incomMessage[200]; //message of person you're comm with  

//check that client specified file name and server host name
if(argc != 2){
printf("Incorrect number of arguments");
return 0;
}

//assigns host name and opens socket
struct hostent *server;
struct sockaddr_in si_server;
server = gethostbyname(argv[1]);

//removes preexisting data; if using IPv4, less bit fields are used so clearning ensures that these extra spots (occupied by IPv6 protocols) contain zeros and not garbage values
bzero((char*)&si_server, sizeof(si_server));

//specifies which protocol family that tells user what kind of addresses our socket can comm with
si_server.sin_family = AF_INET;

//copies byte sequence (copies from hostend structure to sockaddr_in)
bcopy((char*)server->h_addr,(char*)&si_server.sin_addr, server->h_length);

//port number hardcoded to 58619 (numbers below 5000 are automatically assigned by Kernel if you specify port no. 0)
//not bad to let kernel assign you port number but must create interface to comm with kernel 
//if you pick port number, choose between 5k-64k and bigger the number the better because less likely to run into another program using same port number
si_server.sin_port=htons(58619);

//specifications on what kind of socket we want (IPv4, Datagram, Reg. Protocol) ; since IPv4, we must clear preexisting data fields used in IPv6
int sockfd = socket(PF_INET, SOCK_DGRAM, 0); //creates socket

//promts user to for username to chat by)
printf("Enter username (no more than 20 characters long): "); //my username (not of person I'm talking to)
fgets(username, sizeof(username), stdin); //better to use fgets as opposed to scanf for your stdin type because will take in all characters, including through new line \n  

printf("\n"); 

while(1){

//used as argument 6 in recvfrom (must be pointer not just int) 
socklen_t len = sizeof(si_server);
 
//prompts user for message to send
printf("Type message you'd like to send (no more than 200 characters long): ");
fgets(message, sizeof(message), stdin);

printf("\n"); 

//send username to server (localhost here)
sendto(sockfd, username, sizeof(username), 0, (struct sockaddr*)&si_server, sizeof(si_server)); 

//sends message to server
sendto(sockfd, message, sizeof(message), 0, (struct sockaddr*)& si_server, sizeof(si_server)); 

//check for "quit" on my side (assume quit is spelled in lowercase)
if(strcmp(message, "quit\n") ==0){
printf("Nice chatting with you!\n");
break;
}

//receives username from person you're comm with
recvfrom(sockfd, commWith, sizeof(commWith), 0, (struct sockaddr*)&si_server, &len);  

//int nameLength = sizeof(commWith); //gets rid of automatic new line character that'll come with username
//strcpy(commWith[nameLength-1], "\0");  

//receives message of person you're comm with
recvfrom(sockfd, incomMessage, sizeof(incomMessage), 0, (struct sockaddr*)&si_server, &len);

//check for "quit" on friend's side (assume quit is spelled in all lowercase)
if(strcmp(incomMessage, "quit\n") ==0){  //newline char stored in stdin so must add this in comparison!!
printf("Nice chatting with you!\n"); 
break;
}

//prints out chat partner's name and message
printf("%s says: ", commWith); 
printf("%s\n", incomMessage);

} //end while loop
  
return 0; 	
}
