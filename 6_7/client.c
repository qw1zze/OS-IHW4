#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char *server_ip;
char *server_port;

void philosopher_sim(int philosopher_id, int action, int client_socket) {
    char buffer[2];
    buffer[0] = philosopher_id + '0';
    buffer[1] = action + '0';

    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    sendto(client_socket, buffer, sizeof(buffer), 0,
           (const struct sockaddr *)&server_addr, addr_len);

    char response[2];
    int recv = recvfrom(client_socket, response, sizeof(response), 0,
                              (struct sockaddr *)&server_addr, &addr_len);

    if (recv < 0) {
        perror("Response error");
    } else {
        response[recv] = '\0';
        int success = response[0] - '0';
        if (success == 1) {
            if (action == 0) {
                printf("Philosopher %d is eating\nl", philosopher_id);
            } else if (action == 1) {
                printf("Philosopher %d is thinking\n", philosopher_id);
            }
        } else {
            printf("Philosopher %d can't start eat\n", philosopher_id);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Incorrect args, should be <philosopher_id> <server_ip> <port>\n");
        exit(EXIT_FAILURE);
    }

    int philosopher_id = atoi(argv[1]);

    server_ip = argv[2];
    server_port = argv[3];

    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sleep(rand() % 3 + 1);

        philosopher_sim(philosopher_id, 0, client_socket);

        sleep(rand() % 3 + 1);

        philosopher_sim(philosopher_id, 1, client_socket);
    }

    close(client_socket);
    return 0;
}