/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 13:59:22 by thloyan           #+#    #+#             */
/*   Updated: 2023/05/31 16:09:19 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_client	*g_clients = NULL;

void	free_all_exit(void)
{
	t_client	tmp;

	while (g_clients)
	{
		tmp = *g_clients->next;
		free_memory_client(g_clients->pid);
		free(g_clients);
		g_clients = &tmp;
	}
	exit(1);
}

t_client	*find_or_create_client(pid_t pid)
{
	t_client	*client;

	client = g_clients;
	while (client != NULL)
	{
		if (client->pid == pid)
			return (client);
		client = client->next;
	}
	client = create_client(pid);
	client->next = g_clients;
	g_clients = client;
	return (client);
}

void	free_memory(t_data *data)
{
	t_node	*temp;

	while (data->head != NULL)
	{
		temp = data->head;
		data->head = data->head->next;
		free(temp);
	}
}

void	free_memory_client(pid_t pid)
{
	t_client	*client;
	t_client	*prev;

	prev = NULL;
	client = g_clients;
	while (client != NULL)
	{
		if (client->pid == pid)
		{
			if (prev == NULL)
				g_clients = client->next;
			else
				prev->next = client->next;
			free_memory(&client->message_data);
			free(client);
			break ;
		}
		prev = client;
		client = client->next;
	}
}
