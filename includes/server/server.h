/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:23:16 by thloyan           #+#    #+#             */
/*   Updated: 2023/05/31 15:19:11 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# define BLOCK_SIZE 1024
# include <unistd.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>

typedef struct s_node {
	char			data[BLOCK_SIZE];
	struct s_node	*next;
}	t_node;

typedef struct s_data {
	t_node	*head;
	t_node	*tail;
}	t_data;

typedef struct s_client {
	pid_t			pid;
	int				bit_index;
	int				char_index;
	unsigned char	current_char;
	t_data			message_data;
	struct s_client	*next;
}	t_client;

int			ft_strlen(char *str);
void		ft_putstr(char *str);
void		ft_putnbr_fd(int n, int fd);

t_node		*create_node(void);
t_client	*create_client(pid_t pid);
t_client	*find_or_create_client(pid_t pid);
void		free_memory(t_data *data);
void		print_and_reset_data(t_client *client);

#endif