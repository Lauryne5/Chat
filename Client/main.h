
#ifndef MAIN_H
#define MAIN_H

unsigned long receive(void*);
unsigned long sendInput(SOCKET);
int retrievePseudo(char*);
int sendPseudo(SOCKET, char*, int);
void closeConnection(SOCKET);
#endif //MAIN_H
