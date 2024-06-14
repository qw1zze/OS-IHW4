#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int forks[5];

void handle_clients(int server_socket) {
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        int recv = recvfrom(server_socket, buffer, sizeof(buffer), 0,
                                  (struct sockaddr *)&client_addr, &addr_len);
        if (recv < 0) {
            perror("Recvfrom error");
            continue;
        }

        int philosopher_id = buffer[0] - '0';
        int action = buffer[1] - '0';

        if (action == 0) {
            int lfork = philosopher_id;
            int rfork = (philosopher_id + 1) % 5;

            if (forks[lfork] == 0 && forks[rfork] == 0) {
                forks[lfork] = 1;
                forks[rfork] = 1;

                printf("fork %d is locked\n", lfork + 1);
                printf("fork %d is locked\n", rfork + 1);

                sendto(server_socket, "1", 1, 0, (struct sockaddr *)&client_addr, addr_len);
            } else {
                sendto(server_socket, "0", 1, 0, (struct sockaddr *)&client_addr, addr_len);
            }
        } else if (action == 1) {
            int lfork = philosopher_id;
            int rfork = (philosopher_id + 1) % 5;

            if (forks[lfork] == 1 && forks[rfork] == 1) {
                forks[lfork] = 0;
                forks[rfork] = 0;

                printf("fork %d is unlocked\n", lfork + 1);
                printf("fork %d is unlocked\n", rfork + 1);

                sendto(server_socket, "1", 1, 0, (struct sockaddr *)&client_addr, addr_len);
            } else {
                sendto(server_socket, "0", 1, 0, (struct sockaddr *)&client_addr, addr_len);
            }
        } else if (action == 2) {
            // Action 2: Request for forks status
            char forks_status[10]; // Assuming 5 forks (0 or 1 for each fork)

            for (int i = 0; i < 5; ++i) {
                forks_status[i] = forks[i] + '0';
            }
            forks_status[5] = '\0';

            sendto(server_socket, forks_status, strlen(forks_status), 0,
                   (struct sockaddr *)&client_addr, addr_len);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect args, should be <port>\n");
        exit(1);
    }

    char *server_port = argv[1];

    int server_socket;
    struct sockaddr_in server_addr;

    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(server_port));

    if (bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; ++i) {
        forks[i] = 0;
    }

    handle_clients(server_socket);

    close(server_socket);
    return 0;
}