/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:21:05 by thloyan           #+#    #+#             */
/*   Updated: 2023/05/25 16:49:28 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "server.h"

#define BLOCK_SIZE 1024

typedef struct s_node {
    char data[BLOCK_SIZE];
    struct s_node *next;
} t_node;

typedef struct s_data {
    t_node *head;
    t_node *tail;
} t_data;

typedef struct s_client {
    pid_t pid;
    int bit_index;
    int char_index;
	unsigned char current_char;
    t_data message_data;
    struct s_client *next;
} t_client;

t_client *g_clients = NULL;

t_node *create_node(void) {
    t_node *new_node;

    new_node = malloc(sizeof(t_node));
    if (new_node == NULL)
        exit(1);
    new_node->next = NULL;
    return new_node;
}

t_client *create_client(pid_t pid) {
    t_client *new_client;

    new_client = malloc(sizeof(t_client));
    if (new_client == NULL)
        exit(1);
    new_client->pid = pid;
    new_client->bit_index = 0;
    new_client->char_index = 0;
	new_client->current_char = 0;
    new_client->message_data.head = create_node();
    new_client->message_data.tail = new_client->message_data.head;
    new_client->next = NULL;
    return new_client;
}

void free_memory(t_data *data) {
    t_node *temp;

    while (data->head != NULL) {
        temp = data->head;
        data->head = data->head->next;
        free(temp);
    }
}

t_client *find_or_create_client(pid_t pid) {
    t_client *client;

    client = g_clients;
    while (client != NULL) {
        if (client->pid == pid) {
            return client;
        }
        client = client->next;
    }

    client = create_client(pid);
    client->next = g_clients;
    g_clients = client;
    return client;
}

void print_and_reset_data(t_client *client) {
    t_node *iter;

    iter = client->message_data.head;
    while (iter != NULL) {
        ft_putstr(iter->data);
        iter = iter->next;
    }
    ft_putstr("\n");
    free_memory(&client->message_data);
    client->message_data.head = create_node();
    client->message_data.tail = client->message_data.head;
    client->char_index = 0;
}

void handle_received_char(t_client **client)
{
	t_node	*new_node;

	if ((*client)->char_index >= BLOCK_SIZE) {
		new_node = create_node();
		(*client)->message_data.tail->next = new_node;
		(*client)->message_data.tail = new_node;
		(*client)->char_index = 0;
	}
	(*client)->message_data.tail->data[(*client)->char_index] = (*client)->current_char;
	if ((*client)->current_char == '\0')
	{
		print_and_reset_data(*client);
		(*client)->char_index = 0;
		return ;
	}
	(*client)->char_index++;
}

void signal_bit_handler(int signum, siginfo_t *info, void *ucontext) {
    t_client *client;

    (void)ucontext;
    client = find_or_create_client(info->si_pid);

    if (signum == SIGUSR2) {

        client->current_char |= (1 << client->bit_index);
    }
    client->bit_index++;
    if (client->bit_index >= 8) {
		handle_received_char(&client);
		client->current_char = 0;
		client->bit_index = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

void	setup_signal_handler(void)
{
	struct sigaction	sa;
	sigset_t signal_set;


	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_bit_handler;
	sigemptyset(&signal_set);

	sigaddset(&signal_set, SIGUSR1);
    sigaddset(&signal_set, SIGUSR2);

	sa.sa_mask = signal_set;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		exit(1);
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_putnbr_fd(pid, 1);
	setup_signal_handler();
	while (1)
		pause();
	return (0);
}
