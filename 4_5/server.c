#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

const char* forks[5] = {"1", "2", "3", "4", "5"};

void handle_philosopher(int client_socket) {
    int philosopher_id;
    read(client_socket, &philosopher_id, sizeof(int));

    while (1) {
        char request[256];
        read(client_socket, request, sizeof(request));

        if (strcmp(request, "pickup") == 0) {
            int left = philosopher_id;
            int right = (philosopher_id + 1) % 5;

            sem_t *lfork = sem_open(forks[left], 0);
            sem_t *rfork = sem_open(forks[right], 0);

            sem_wait(lfork);
            sem_wait(rfork);

            printf("fork %d is locked\n", left + 1);
            printf("fork %d is locked\n", right + 1);

            sem_close(lfork);
            sem_close(rfork);

            char response[] = "eating";
            write(client_socket, response, sizeof(response));
        } else if (strcmp(request, "putdown") == 0) {
            int left = philosopher_id;
            int right = (philosopher_id + 1) % 5;

            sem_t *lfork = sem_open(forks[left], 0);
            sem_t *rfork = sem_open(forks[right], 0);

            sem_post(lfork);
            sem_post(rfork);

            printf("fork %d is unlocked\n", left + 1);
            printf("fork %d is unlocked\n", right + 1);

            sem_close(lfork);
            sem_close(rfork);

            char response[] = "thinking";
            write(client_socket, response, sizeof(response));
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Incorrect args, should be <server_ip> <port>\n");
        exit(1);
    }

    char *server_ip = argv[1];
    char *server_port = argv[2];

    for (int i = 0; i < 5; ++i) {
        sem_unlink(forks[i]);
        sem_t *fork = sem_open(forks[i], O_CREAT, 0644, 1);
        sem_close(fork);
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        exit(1);
    }
    listen(server_socket, 5);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        pthread_t thread;
        pthread_create(&thread, NULL, (void*)handle_philosopher, (void*)(intptr_t)client_socket);
    }

    close(server_socket);
    return 0;
}