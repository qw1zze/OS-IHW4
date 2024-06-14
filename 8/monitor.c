#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char *server_ip;
char *server_port;

void monitor_handle(int client_socket) {
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    char request[2];
    request[1] = '2';

    sendto(client_socket, request, sizeof(request), 0,
           (const struct sockaddr *)&server_addr, addr_len);

    char response[10];
    int recv = recvfrom(client_socket, response, sizeof(response), 0,
                        (struct sockaddr *)&server_addr, &addr_len);

    if (recv < 0) {
        perror("Response error");
    } else {
        response[recv] = '\0';
        printf("Forks status: %s\n", response);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Incorrect args, should be <server_ip> <port>\n");
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    server_port = argv[2];

    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sleep(3);
        monitor_handle(client_socket);
    }

    close(client_socket);
    return 0;
}