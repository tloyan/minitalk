/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:21:08 by thloyan           #+#    #+#             */
/*   Updated: 2023/04/03 14:32:38 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static int	g_ack_received = 0;

void	ack_handler(int signum)
{
	(void)signum;
	g_ack_received = 1;
}

void	send_bit(pid_t server_pid, unsigned char bit)
{
	int	signum;

	if (bit)
		signum = SIGUSR2;
	else
		signum = SIGUSR1;
	kill(server_pid, signum);
	while (!g_ack_received)
		pause();
	g_ack_received = 0;
}

void	setup_ack_handler(void)
{
	struct sigaction	sa;

	sa.sa_flags = 0;
	sa.sa_handler = ack_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("Erreur en configurant le gestionnaire de signal");
		exit(1);
	}
}

void	send_message(pid_t server_pid, char *message)
{
	unsigned char	c;
	int				i;
	int				j;

	i = 0;
	while (i <= (int)strlen(message))
	{

		c = message[i];
		j = 0;
		while (j < 8)
		{
			send_bit(server_pid, c & 1);
			c >>= 1;
			j++;
		}
		i++;
	}
}

int	main(int argc, char *argv[])
{
	pid_t	server_pid;
	char	*message;

	if (argc != 3)
	{
		printf("Usage: %s <server_pid> <message>\n", argv[0]);
		exit(1);
	}
	server_pid = atoi(argv[1]);
	message = argv[2];
	setup_ack_handler();
	send_message(server_pid, message);
	return (0);
}
