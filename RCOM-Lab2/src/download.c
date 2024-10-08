#include "../includes/download.h"



const char* parseFileName(const char* path){
    char *aux = strrchr(path, '/');
    if (aux == NULL) {
        fprintf(stderr, "Invalid URL\n");
        exit(-1);
    }
    return aux + 1;
}


void parseInfo(const char *url, char *hostname, char *path){

    char *aux = strstr(url, "://");

    if (aux == NULL) {
        fprintf(stderr, "Invalid URL\n");
        exit(-1);
    }

    aux += 3;
    char *aux2 = strstr(aux, "/");

    if (aux2 == NULL) {
        fprintf(stderr, "Invalid URL\n");
        exit(-1);
    }

    int size = aux2 - aux;
    strncpy(hostname, aux, size);
    hostname[size] = '\0';
    strcpy(path, aux2);
}

char* getIp(const char *hostname){

    struct hostent *h;

    if ((h = gethostbyname(hostname)) == NULL) {
        fprintf(stderr, "Invalid hostname '%s'\n", hostname);
        exit(-1);
    }

    return inet_ntoa(*((struct in_addr *) h->h_addr));

}

int connectSocket(const char *ip, int port){

    int sockfd;
    struct sockaddr_in server_addr;


    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);   
    server_addr.sin_port = htons(port);       

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }
    return sockfd;

}

void checkUsername(int controlSocket){
    
    char command[MAX_COMMAND_SIZE];
    size_t bytes;

    char username[MAX_INPUT];

    printf("\nUsername: ");
    scanf("%s", username);

    sprintf(command, "USER %s\r\n", username);

    bytes = send(controlSocket, command, strlen(command), 0);

    if (bytes == -1) {
        perror("Error: Could not send the username\n");
        exit(-1);
    }

    bytes = recv(controlSocket, command, sizeof(command), 0);
    command[bytes] = '\0';
    printf("\nServer Response: %s", command);

    if (strstr(command, "331") == NULL) {
        perror("Invalid username");
        exit(-1);
    }
    
}

void checkPassword(int controlSocket){
    
        char command[MAX_COMMAND_SIZE];
        size_t bytes;

         char password[MAX_INPUT];

    
        printf("\nPassword: ");
        scanf("%s", password);
    
        sprintf(command, "PASS %s\r\n", password);

        bytes = send(controlSocket, command, strlen(command), 0);
    
        if (bytes == -1) {
            perror("Error: Could not send the password\n");
            exit(-1);
        }
    
        // message from server
        bytes = recv(controlSocket, command, sizeof(command), 0);
        command[bytes] = '\0';
        printf("\nServer Response: %s", command);
    
        if (strstr(command, "230") == NULL) {
            perror("Invalid password");
            exit(EXIT_FAILURE);
        }
}


void loginToHost(int controlSocket) {
    char command[MAX_COMMAND_SIZE];
    size_t bytes;

    // message from server
    bytes = recv(controlSocket, command, sizeof(command), 0);
    command[bytes] = '\0';
    printf("\nServer Response: %s", command);

    checkUsername(controlSocket);
    checkPassword(controlSocket);
}


int setPassiveMode(int controlSocket){
    char command[MAX_COMMAND_SIZE];
    size_t bytes;

    // PASV command
    sprintf(command, "PASV\r\n");
    bytes = send(controlSocket, command, strlen(command), 0);

    if (bytes == -1) {
        perror("Error: Could not send the PASV command\n");
        exit(-1);
    }

     bytes = recv(controlSocket, command, sizeof(command), 0);
    command[bytes] = '\0';
    printf("\nServer Response: %s", command);

    char *token;
    unsigned int ip_parts[4], port_parts[2];

    // Remove the initial part of the response
    token = strtok(command, "(");

    // Tokenize the IP and port parts
    int i = 0;
    token = strtok(NULL, ",");
    while(token != NULL && i < 6) {
        if(i < 4) {
            ip_parts[i] = atoi(token);
        } else {
            port_parts[i - 4] = atoi(token);
        }
        token = strtok(NULL, ",");
        i++;
    }

    return port_parts[0] * 256 + port_parts[1];
}

void downloadFile(int controlSocket, int dataSocket, const char* path){
    char command[256];
    size_t bytes;
     // Send the retrieve command to the server
    
    sprintf(command, "RETR %s\r\n", path);
    bytes = send(controlSocket, command, strlen(command), 0);

    if (bytes == -1) {
        perror("Error: Could not send the retrieve command\n");
        exit(-1);
    }

    // Check the response in the control socket
    bytes = recv(controlSocket, command, sizeof(command), 0);

    if (bytes == -1) {
        perror("Error: Could not read the control socket\n");
        exit(-1);
    }

    command[bytes] = '\0';
    printf("\nServer Response: %s", command);

    int statCode;
    sscanf(command, "%d", &statCode);

    if(statCode != 150) {
        perror("Error: Could not retrieve the file\n");
        exit(-1);
    }

    // Open the file to write the data
    FILE *file = fopen(parseFileName(path), "wb");
    if (file == NULL) {
        perror("Error: Could not open the file\n");
        exit(-1);
    }

    printf("\nDownloading file...\n");

    char buffer[MAX_COMMAND_SIZE];
    ssize_t receivedBytes;

    while ((receivedBytes = recv(dataSocket, buffer, sizeof(buffer), 0)) > 0) {
        if (fwrite(buffer, 1, receivedBytes, file) != receivedBytes) {
            perror("Error: Could not write to the file\n");
            exit(-1);
        }
    }

   
    bytes = recv(controlSocket, command, sizeof(command), 0);

    if (bytes == -1) {
        perror("Error: Could not read the control socket\n");
        exit(-1);
    }

    command[bytes] = '\0';
    printf("\nServer Response: %s", command);
    
    sscanf(command, "%d", &statCode);

    if(statCode != 226) {
        perror("Error: Could not retrieve the file\n");
        exit(-1);
    }

    fclose(file);
}

    



int main(int argc, char *argv[]) {

    if (argc != 2) {
    fprintf(stderr, "Usage: %s <url>\n", argv[0]);
    exit(-1);
    }   

    char hostname[256];
    char path[256];

    parseInfo(argv[1], hostname, path);

    printf("\nHostname: %s\n", hostname);
    printf("Path: %s\n", path);

    char *serverIp = getIp(hostname);

     if (serverIp == NULL) {
        perror("Error: Could not get the ip address\n");
        exit(-1);
     }

    printf("\nIP Address is %s.\n", serverIp);

    int controlSocket = connectSocket(serverIp, CTRL_PORT);

    loginToHost(controlSocket);

    // Enter passive mode and get the data port
    int port = setPassiveMode(controlSocket);
    printf("\nData Port: %d\n", port);

    int dataSocket = connectSocket(serverIp, port);

    downloadFile(controlSocket, dataSocket, path);

    close(controlSocket);
    close(dataSocket);
    
    return 0;

}