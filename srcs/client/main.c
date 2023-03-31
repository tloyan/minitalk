/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:21:08 by thloyan           #+#    #+#             */
/*   Updated: 2023/03/31 16:14:53 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int ack_received = 0;

void ack_handler(int signum) {
    (void)signum;
    ack_received = 1;
}

void send_bit(pid_t server_pid, unsigned char bit) {
    int signum = bit ? SIGUSR2 : SIGUSR1;
    kill(server_pid, signum);
    while (!ack_received) {
        pause();
    }
    ack_received = 0;
}

void setup_ack_handler() {
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = ack_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Erreur en configurant le gestionnaire de signal");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_pid> <message>\n", argv[0]);
        exit(1);
    }

    pid_t server_pid = atoi(argv[1]);
    char *message = argv[2];

    setup_ack_handler();

	for (int i = 0; i < (int)strlen(message);) {
        unsigned char c = message[i];
        int len = 1;
        if ((c & 0x80) == 0) {
            len = 1;
        } else if ((c & 0xE0) == 0xC0) {
            len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            len = 4;
        }

        for (int k = 0; k < len; k++) {
            unsigned char byte = message[i + k];
            for (int j = 0; j < 8; j++) {
                send_bit(server_pid, byte & 1);
                byte >>= 1;
            }
        }
        i += len;
    }

    for (int i = 0; i <= (int)strlen(message); i++) {
        unsigned char c = message[i];
        for (int j = 0; j < 8; j++) {
            send_bit(server_pid, c & 1);
            c >>= 1;
        }
    }

    return 0;
}
