/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:04:31 by thloyan           #+#    #+#             */
/*   Updated: 2023/05/31 16:20:51 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_node	*create_node(void)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (new_node == NULL)
		free_all_exit();
	new_node->next = NULL;
	return (new_node);
}

t_client	*create_client(pid_t pid)
{
	t_client	*new_client;

	new_client = malloc(sizeof(t_client));
	if (new_client == NULL)
		free_all_exit();
	new_client->pid = pid;
	new_client->bit_index = 0;
	new_client->char_index = 0;
	new_client->current_char = 0;
	new_client->message_data.head = create_node();
	new_client->message_data.tail = new_client->message_data.head;
	new_client->next = NULL;
	return (new_client);
}
