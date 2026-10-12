#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "list.h"

#define PORT 9001
#define ACK "ACK"

// Global pointer so the signal handler can free the list on exit
static list_t *mylist = NULL;

void sigint_handler(int sig) {
    if (mylist != NULL) {
        list_free(mylist);
        mylist = NULL;
    }
    printf("\nServer terminated gracefully.\n");
    exit(0);
}

int main(int argc, char const *argv[]) {
    int n, val, idx;
    char buf[1024];
    char sbuf[1024];
    char *token;
    struct sockaddr_in servAddr;

    // Register signal handler for Ctrl-C graceful termination
    signal(SIGINT, sigint_handler);

    // Create server socket
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure address and port reuse
    int opt = 1;
    setsockopt(servSockD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for client connection
    listen(servSockD, 1);

    // Accept incoming connection
    int clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0) {
        perror("Accept failed");
        exit(1);
    }

    // Allocate linked list
    mylist = list_alloc();

    while (1) {
        memset(buf, '\0', sizeof(buf));
        memset(sbuf, '\0', sizeof(sbuf));

        n = recv(clientSocket, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            // Client disconnected
            break;
        }
        buf[n] = '\0';

        token = strtok(buf, " \t\r\n");
        if (token == NULL) {
            continue;
        }

        if (strcmp(token, "exit") == 0) {
            list_free(mylist);
            mylist = NULL;
            close(clientSocket);
            close(servSockD);
            exit(0);
        } else if (strcmp(token, "get_length") == 0) {
            val = list_length(mylist);
            sprintf(sbuf, "%s%d", "Length = ", val);
        } else if (strcmp(token, "add_front") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token != NULL) {
                val = atoi(token);
                list_add_to_front(mylist, val);
                sprintf(sbuf, "%s%d", ACK, val);
            }
        } else if (strcmp(token, "add_back") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token != NULL) {
                val = atoi(token);
                list_add_to_back(mylist, val);
                sprintf(sbuf, "%s%d", ACK, val);
            }
        } else if (strcmp(token, "add_position") == 0) {
            token = strtok(NULL, " \t\r\n");
            idx = atoi(token);
            token = strtok(NULL, " \t\r\n");
            val = atoi(token);
            list_add_at_index(mylist, idx, val);
            sprintf(sbuf, "%s%d", ACK, val);
        } else if (strcmp(token, "remove_front") == 0) {
            val = list_remove_from_front(mylist);
            sprintf(sbuf, "%d", val);
        } else if (strcmp(token, "remove_back") == 0) {
            val = list_remove_from_back(mylist);
            sprintf(sbuf, "%d", val);
        } else if (strcmp(token, "remove_position") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token != NULL) {
                idx = atoi(token);
                val = list_remove_at_index(mylist, idx);
                sprintf(sbuf, "%d", val);
            }
        } else if (strcmp(token, "get") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token != NULL) {
                idx = atoi(token);
                val = list_get_elem_at(mylist, idx);
                sprintf(sbuf, "%d", val);
            }
        } else if (strcmp(token, "print") == 0) {
            char *listStr = listToString(mylist);
            sprintf(sbuf, "%s", listStr);
        } else {
            sprintf(sbuf, "Unknown command");
        }

        // Send response back to client
        send(clientSocket, sbuf, strlen(sbuf) + 1, 0);
    }

    if (mylist != NULL) {
        list_free(mylist);
        mylist = NULL;
    }
    close(clientSocket);
    close(servSockD);
    return 0;
}