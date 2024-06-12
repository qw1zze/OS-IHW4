#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

int philosopher_id;

bool running = true;

void end_program(int signal) {
    if (signal == SIGINT) {
        printf("Ending program\n");
        running = false;
    }

}

void think() {
    printf("Philosopher %d is thinking\n", philosopher_id);
    sleep(rand() % 3 + 1);
}

void eat() {
    printf("Philosopher %d is eating\n", philosopher_id);
    sleep(rand() % 3 + 1);
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Incorrect args, should be <server_ip> <port> <philosopher_id>\n");
        return 1;
    }

    signal(SIGINT, end_program);

    char *server_ip = argv[1];
    char *server_port = argv[2];
    philosopher_id = atoi(argv[3]);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect error");
        exit(1);
    }
    write(client_socket, &philosopher_id, sizeof(int));

    while (running) {
        think();

        char request[] = "pickup";
        write(client_socket, request, sizeof(request));
        char response[256];
        read(client_socket, response, sizeof(response));

        if (strcmp(response, "eating") == 0) {
            eat();

            char putdown[] = "putdown";
            write(client_socket, putdown, sizeof(putdown));
            read(client_socket, response, sizeof(response));
        }
    }

    close(client_socket);
}
