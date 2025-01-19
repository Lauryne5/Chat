#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

unsigned long receive(void*);
unsigned long sendInput(SOCKET);

int main(void) {

    WSADATA wsaData;
    SOCKET client;

    HANDLE rcvThread;
    HANDLE sendThread;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(3000);
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); //TODO



    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup initialization failed\n");
        return -1;
    }

    if((client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("socket failed\n");
        return -1;
    }

    if (connect(client, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    if ((rcvThread = CreateThread(NULL, 0, receive, &client, 0, NULL)) == NULL){
        printf("Failed to create receive thread\n");
        return -1;
    }
    sendInput(client);

    return 0;
}

unsigned long receive(void* arg) {
    int buffersize = 32;
    char buffer[buffersize];

    int receivesize = 0;
    int receivemax;
    int i;

    while (1) {
        char *array = malloc(buffersize*sizeof(char));
        receivemax = 0;
        i = 0;

        while ((receivesize = recv(*(SOCKET*)arg, buffer, strlen(buffer), 0)) <= 0) {
            int index = buffersize*(i)-i;
            if (index <= -1)
                index = 0;
            memcpy(array+index, buffer, buffersize);
            i++;
            array = realloc(array, (buffersize+31*(i))*sizeof(char));
            receivemax+= receivesize;
        }
        array[receivemax] = '\0';
        printf("%s", array);
        free(array);
    }

    return 0;
}

unsigned long sendInput(SOCKET arg) {
    int size = 32;
    char buffer[size];

    while (1) {
        char* array = malloc(size*sizeof(char));
        int i = 0;
        while (fgets(buffer, size, stdin) != NULL) {
            char *end = strchr(buffer, '\n');
            int index = size*(i)-i;
            if(end != NULL){
                memcpy(array + index, buffer, end - buffer);
                array[index+end-buffer] = '\0';
                break;
            }
            if (index <= -1)
                index = 0;
            memcpy(array+index, buffer, size);
            i++;
            array = realloc(array, (size+31*(i))*sizeof(char));
        }
        free(array);
        if (send(arg, array, strlen(array), 0) == SOCKET_ERROR) {
            printf("Cannot send message\n");
            //TODO
        }
    }
    return 0;
}