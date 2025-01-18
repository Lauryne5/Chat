//Faire un chat entre plusieurs clients avec un serveur distribuant les messages
#include "main.h"

Client* clients[MAX_CHATROOM_SIZE] = {NULL};
HANDLE threadlist[MAX_CHATROOM_SIZE] = {NULL};
SOCKET server;

int main(void) {

    WSADATA wsaddata;
    HANDLE serverThread;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(3000);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");



    if (WSAStartup(MAKEWORD(2,2), &wsaddata) != 0) {
        printf("Initialization failed\n");
        return -1;
    }
    if ((server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf(("Failed to create socket"));
        return -1;
    }
    if (bind(server, (struct sockaddr*)&address, 0) != 0) {
        printf("Failed to bind\n");
        return -1;
    }
    if (listen(server, 14) == SOCKET_ERROR) {
        printf("Failed to listen to the port\n");
        return -1;
    }

    if ((serverThread = CreateThread(NULL, 0, handleNewClients, &server, 0, NULL)) == NULL){
        printf("Couldn't start server thread\n");
        //TODO
    }

    return 0;
}

int addClient(SOCKET client, struct sockaddr_in* clientaddress) {

    int index = -1;
    for (int i = 0; i < MAX_CHATROOM_SIZE; i++) {
        if (clients[i] != NULL) {
            if (clients[i]->addressip->sin_addr.s_addr == clientaddress->sin_addr.s_addr) {
                printf("This client already exists\n");
                return -2;
            }
        }
        else {
            if (index == -1)
                index = i;
        }
    }
    if (index == -1) {
        printf("Trop de clients simultanes\n");
        return -3;
    }
    Client* newclient = malloc(sizeof(Client*));
    if (newclient == NULL) {
        printf("failed to allocate memory to client\n");
        exit(1);
    }
    newclient->s = client;
    newclient->addressip = clientaddress;

    clients[index] = newclient;
    return index;
}

unsigned long handleClient(void* i) {

    int index = *(int*)i;
    char* message;

    if (index < 0) {
        return 4;
    }
    while (1) {
        if (receiveStringFromClient(&message, clients[index]->s) < 0) {
            printf("No data received, closing socket\n");
            removeClient(index);
            return 1;
        }
        if (sendToEveryone(index, message) != 0) {
            printf("Couldn't send to everyone\n");
            //TODO
        }
    }
    return 0;

}

int receiveStringFromClient(char** array, SOCKET client) {
    int buffersize = 32;
    char buffer[buffersize];
    *array = malloc(buffersize*sizeof(char));
    int i = 0;
    int receivemax = 0;
    int receivesize = 0;

    while((receivesize = recv(client, buffer, strlen(buffer), 0)) <= 0){
        int index = buffersize*(i)-i;
        if (index <= -1)
            index = 0;
        memcpy(*array+index, buffer, buffersize);
        i++;
        *array = realloc(*array, (buffersize+31*(i))*sizeof(char));
        receivemax += receivesize;
    }
    *array[receivemax] = '\0';

    return receivesize;
}

int sendToEveryone(int index, char* message) {

    for (int i =0; i < MAX_CHATROOM_SIZE; i++) {
        if (index == i) {
            continue;
        }
        if (send(clients[i]->s, message, strlen(message), 0) == SOCKET_ERROR) {
            printf("Sending data failed\n");
            return -1;
        }

    }
    return 0;
}

void removeClient(int index) {

    if (closesocket(clients[index]->s) == SOCKET_ERROR) {
        printf("Failed to closed socket\n");
    }
    free(clients[index]);
    clients[index] = NULL;

}

int addThreadToList(HANDLE thread) {
    for (int i =0; i < MAX_CHATROOM_SIZE; i++) {
        if (threadlist[i] == NULL) {
            threadlist[i] = thread;
            return 0;
        }
    }
    return -1;
}

unsigned long handleNewClients(void *) {

    SOCKET temporaryClient;
    HANDLE clientThread;
    struct sockaddr* temporaryClientIp = NULL;
    char* connectionResponse = "Connecte a la chatroom";
    int index = -1;
    int err = -1;

    while (1){
        if (temporaryClient = accept(server, temporaryClientIp, NULL) != INVALID_SOCKET) {
            err = WSAGetLastError();
            if (err == WSAENETDOWN || err == WSAENOBUFS)
                exit(1);
            continue;
        }

        puts("Connection accepted\n");
        if ((index = addClient(temporaryClient,  (struct sockaddr_in*)temporaryClientIp)) < 0){
            printf("Client could not be added\n");
            closesocket(temporaryClient);
            continue;
        }
        send(temporaryClient, connectionResponse, strlen(connectionResponse),0);

        if ((clientThread = CreateThread(NULL, 0, handleClient, &index, 0, NULL)) == NULL) {
            printf("Failed to create client thread\n");
            //TODO
        }

        addThreadToList(clientThread);
    }

    return 0;
}

void readUserInput() {

    while (1) {
        int sizelist = 0;
        char c;
        char* input = NULL, *command = NULL, *parameters = NULL;
        Node *head = malloc(sizeof(Node*));
        head->next = NULL;

        while((c = fgetc(stdin))!= EOF){
            if(c == '\n')
                break;
            put(head, c);
            sizelist++;
        }
        sizelist++;

        if (sizelist >= 512) {
            printf("Error Input too long");
            emptyList(head);
            continue;
        }
        input = getInput(head, sizelist);

        if (input == NULL)
            continue;
        splitInput(input, sizelist, &command, &parameters);
        executeCommand(command, parameters);

        emptyList(head);
        free(input);
        input = NULL;

    }

}

void put(Node* n, char c){
    while(n->next != NULL){
        n = n->next;
    }
    Node *last = malloc(sizeof(Node));
    last->next = NULL;

    n->letter = c;
    n->next = last;
}

void emptyList(Node* head) {
    while (head != NULL) {
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}

char* getInput(Node* head, int size) {
    if (head == NULL || size <= 0)
        return NULL;

    char* array = malloc(sizeof(char)*size);
    for (int i = 0; i < size-1; i++) {
        if (head == NULL)
            break;
        array[i] = head->letter;
        head = head->next;
    }
    array[size-1] = '\0';
    return array;
}

int splitInput(char* input, int size, char** command, char** parameter) {

    if (input == NULL)
        return -1;

    *command = malloc(sizeof(char)*size);
    int i = 0;

    for (i; i < size; i++) {
        if (input[i] == ' ')
            break;

        *command[i] = input[i];
    }
    if (size != i)
        *parameter = input+i;

    return 0;
}