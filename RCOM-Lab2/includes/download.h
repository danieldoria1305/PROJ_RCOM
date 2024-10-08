#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define CTRL_PORT 21
#define MAX_INPUT 100
#define MAX_COMMAND_SIZE 1024



//Given the control_socket, do the login process
void loginToHost(int controlSocket);

//Given a hostname returns the IP address (based on getip.c file given on class)
char* getIp(const char *hostname);

//Given an URL returns the hostname and the path
void parseInfo(const char *url, char *hostname, char *path);

//Given a path returns the filename
const char* parseFileName(const char* path);

//Given an ip adress and a port returns the socket (based on clientTCP.c from class)
int connectSocket(const char *ip, int port);

//Password check
void checkPassword(int controlSocket);

//Username check
void checkUsername(int controlSocket);

//Given the control_socket sets the mode to passive
int setPassiveMode(int controlSocket);

//Given control_socket, dataSocket and the path, downloads the file
void downloadFile(int controlSocket, int dataSocket, const char* path);













