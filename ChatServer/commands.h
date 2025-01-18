//
// Created by laury on 18/01/2025.
//

#ifndef COMMANDS_H
#define COMMANDS_H

#include "main.h"

int exitServer(char*);
int executeCommand(char*, char*);

typedef struct Command Command;
struct Command {
    char* name;
    int(*func)(char*);
};

extern Command commands[];

#endif //COMMANDS_H
