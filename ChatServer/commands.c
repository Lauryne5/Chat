#include "commands.h"

Command commands[] = {
    {"exit", exitServer }
};

int exitServer(char* args) {
    char* message = "Chatroom closed";
    for (int i = 0; i < MAX_CHATROOM_SIZE; i++) {
        send(clients[i]->s, message, strlen(message), 0);
        removeClient(i);
        CloseHandle(threadlist[i]);
    }
    closesocket(server);
    exit(0);
}

int executeCommand(char* command, char* parameters) {
    int size = sizeof(commands) / sizeof(Command);
    int result = -1;
    for (int i = 0; i < size; i++) {
        if (strcmp(command, commands[i].name) == 0) {
            result = commands[i].func(parameters);
        }
    }
    if (result == -1) {
        printf("Unknwown command\n");
        return -1;
    }
    if (result != 0) {
        printf("Error"); //TODO
        return -1;
    }

    return 0;
}
