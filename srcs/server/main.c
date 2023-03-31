/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:21:05 by thloyan           #+#    #+#             */
/*   Updated: 2023/03/31 16:15:05 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MESSAGE_SIZE 1024

typedef struct {
    char message[MESSAGE_SIZE];
    int index;
    int bit_index;
    unsigned char current_char;
} ServerData;

ServerData server_data = { .message = {0}, .index = 0, .bit_index = 0, .current_char = 0 };

void signal_bit_handler(int signum, siginfo_t *info, void *ucontext) {
    (void)ucontext;

    if (signum == SIGUSR2) {
        server_data.current_char |= (1 << server_data.bit_index);
    }
    server_data.bit_index++;

    if (server_data.bit_index >= 8) {
        server_data.message[server_data.index++] = server_data.current_char;
        if (server_data.current_char == '\0') {
            printf("Message reçu: %s\n", server_data.message);
            server_data.index = 0;
        }
        server_data.current_char = 0;
        server_data.bit_index = 0;
    }

    // Send confirmation signal to client
    kill(info->si_pid, SIGUSR1);
}

void setup_signal_handler() {
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signal_bit_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("Erreur en configurant le gestionnaire de signal");
        exit(1);
    }
}

int main() {
    pid_t pid = getpid();
    printf("PID du serveur : %d\n", pid);

    setup_signal_handler();

    while (1) {
        pause();
    }

    return 0;
}
