//
// Created by laury on 18/01/2025.
//

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "commands.h"

#define MAX_CHATROOM_SIZE 14

// Struct
typedef struct Client Client;
struct Client{
    SOCKET s;
    struct sockaddr_in* addressip;
};

typedef struct Node Node;
struct Node {
    char letter;
    Node *next;
};

extern Client* clients[];
extern HANDLE threadlist[];
extern SOCKET server;

//Signatures Fonctions
int addClient(SOCKET, struct sockaddr_in*);
unsigned long handleClient(void *);
int receiveStringFromClient(char**, SOCKET);
int sendToEveryone(int, char*);
void removeClient(int);
int addThreadToList(HANDLE);
unsigned long handleNewClients(void *);
void put(Node*, char);
void emptyList(Node*);
char* getInput(Node*, int);
int splitInput(char*, int, char**, char**);
void readUserInput();

#endif //MAIN_H
