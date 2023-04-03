/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:21:05 by thloyan           #+#    #+#             */
/*   Updated: 2023/04/03 14:33:15 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define BLOCK_SIZE 1024

typedef struct s_node
{
	char			data[BLOCK_SIZE];
	struct s_node	*next;
}	t_node;

typedef struct s_data
{
	t_node	*head;
	t_node	*tail;
}	t_data;

t_data	g_data;

void	free_memory(void)
{
	t_node	*temp;

	while (g_data.head != NULL)
	{
		temp = g_data.head;
		g_data.head = g_data.head->next;
		free(temp);
	}
}

t_node	*create_node(void)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (new_node == NULL)
	{
		perror("Erreur d'allocation de mémoire");
		exit(1);
	}
	new_node->next = NULL;
	return (new_node);
}

void	print_and_reset_data(void)
{
	t_node	*iter;

	iter = g_data.head;
	while (iter != NULL)
	{
		printf("%s", iter->data);
		iter = iter->next;
	}
	printf("\n");
	free_memory();
	g_data.head = NULL;
	g_data.tail = NULL;
}

void	handle_received_char(int *index, unsigned char current_char)
{
	t_node	*new_node;

	if (*index >= BLOCK_SIZE)
	{
		new_node = create_node();
		g_data.tail->next = new_node;
		g_data.tail = new_node;
		*index = 0;
	}
	g_data.tail->data[*index] = current_char;
	if (current_char == '\0')
	{
		print_and_reset_data();
		*index = 0;
		return ;
	}
	(*index)++;
}

void	signal_bit_handler(int signum, siginfo_t *info, void *ucontext)
{
	static int				index = 0;
	static int				bit_index = 0;
	static unsigned char	current_char = 0;

	(void)ucontext;
	if (g_data.head == NULL)
	{
		g_data.head = create_node();
		g_data.tail = g_data.head;
	}
	if (signum == SIGUSR2)
		current_char |= (1 << bit_index);
	bit_index++;
	if (bit_index >= 8)
	{
		handle_received_char(&index, current_char);
		current_char = 0;
		bit_index = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

void	setup_signal_handler(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_bit_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		perror("Erreur en configurant le gestionnaire de signal");
		exit(1);
	}
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	printf("PID du serveur : %d\n", pid);
	setup_signal_handler();
	while (1)
		pause();
	free_memory();
	return (0);
}
