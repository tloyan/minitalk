/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:21:05 by thloyan           #+#    #+#             */
/*   Updated: 2023/05/31 16:20:04 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	print_and_reset_data(t_client *client)
{
	t_node	*iter;

	iter = client->message_data.head;
	while (iter != NULL)
	{
		ft_putstr(iter->data);
		iter = iter->next;
	}
	ft_putstr("\n");
	free_memory_client(client->pid);
}

void	handle_received_char(t_client **client)
{
	t_node	*new_node;

	if ((*client)->char_index >= BLOCK_SIZE)
	{
		new_node = create_node();
		(*client)->message_data.tail->next = new_node;
		(*client)->message_data.tail = new_node;
		(*client)->char_index = 0;
	}
	(*client)->message_data.tail->data[(*client)->char_index]
		= (*client)->current_char;
	if ((*client)->current_char == '\0')
	{
		print_and_reset_data(*client);
		*client = NULL;
		return ;
	}
	(*client)->char_index++;
}

void	signal_bit_handler(int signum, siginfo_t *info, void *ucontext)
{
	t_client	*client;

	(void)ucontext;
	client = find_or_create_client(info->si_pid);
	if (signum == SIGUSR2)
		client->current_char |= (1 << client->bit_index);
	client->bit_index++;
	if (client->bit_index >= 8)
	{
		handle_received_char(&client);
		if (client)
		{
			client->current_char = 0;
			client->bit_index = 0;
		}
	}
	kill(info->si_pid, SIGUSR1);
}

void	setup_signal_handler(void)
{
	struct sigaction	sa;
	sigset_t			signal_set;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_bit_handler;
	sigemptyset(&signal_set);
	sigaddset(&signal_set, SIGUSR1);
	sigaddset(&signal_set, SIGUSR2);
	sa.sa_mask = signal_set;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		free_all_exit();
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
