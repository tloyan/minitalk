/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 14:23:18 by thloyan           #+#    #+#             */
/*   Updated: 2023/06/01 12:59:49 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <unistd.h>

int		ft_strlen(char *str);
void	ft_putstr(char *str);
void	ft_putnbr_fd(int n, int fd);
int		ft_atoi(const char *nptr);

#endif