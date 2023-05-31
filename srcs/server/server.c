/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 13:59:22 by thloyan           #+#    #+#             */
/*   Updated: 2023/05/31 15:17:49 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_client	*g_clients = NULL;

t_node	*create_node(void)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (new_node == NULL)
		exit(1);
	new_node->next = NULL;
	return (new_node);
}

t_client	*create_client(pid_t pid)
{
	t_client	*new_client;

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
	return (new_client);
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
	free_memory(&client->message_data);
	client->message_data.head = create_node();
	client->message_data.tail = client->message_data.head;
	client->char_index = 0;
}
