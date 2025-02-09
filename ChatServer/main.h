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
    char* pseudo;
};

typedef struct Node Node;
struct Node {
    char letter;
    Node *next;
};

enum ASCII_Table_Const {
    NUMBER_LOWER_EDGE = 48,
    NUMBER_HIGHER_EDGE = 57,
    UP_CHAR_LOWER_EDGE = 65,
    UP_CHAR_HIGHER_EDGE = 90,
    LO_CHAR_LOWER_EDGE = 97,
    LO_CHAR_HIGHER_EDGE = 122
};

extern Client* clients[];
extern HANDLE threadlist[];
extern SOCKET server;

//Signatures Fonctions
int addClient(SOCKET, struct sockaddr_in*, char*);
unsigned long handleClient(void *);
int receiveStringFromClient(char**, SOCKET, int);
int sendToEveryone(int, char*);
void removeClient(int);
int addThreadToList(HANDLE);
unsigned long handleNewClients(void *);
void put(Node*, char);
void emptyList(Node*);
char* getInput(Node*, int);
int splitInput(char*, int, char**, char**);
void readUserInput();
int retrievePseudo(char**, SOCKET);
#endif //MAIN_H
