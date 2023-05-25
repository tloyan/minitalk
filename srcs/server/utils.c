/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 15:52:38 by thloyan           #+#    #+#             */
/*   Updated: 2023/05/25 16:50:40 by thloyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include <limits.h>

// int	ft_isspace(int c)
// {
// 	if (c == 32 || (c >= 9 && c <= 13))
// 		return (1);
// 	return (0);
// }

// int	ft_isdigit(int c)
// {
// 	if (c >= 48 && c <= 57)
// 		return (1);
// 	return (0);
// }

int	ft_intlen(int n)
{
	int	len;

	len = 1;
	while (n / 10)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	unsigned int	l;

	l = ft_strlen(s);
	write(1, s, l);
}

void	ft_putnbr_fd(int n, int fd)
{
	char	buffer[12];
	int		len;
	int		is_negative;

	len = ft_intlen(n);
	is_negative = 0;
	if (n == INT_MIN)
		return (write(fd, "-2147483648", 11), (void)0);
	if (n < 0)
	{
		is_negative = 1;
		n = n * -1;
		buffer[0] = '-';
	}
	buffer[len + is_negative] = 0;
	while (len-- > 0)
	{
		buffer[len + is_negative] = ((n % 10) + '0');
		n = n / 10;
	}
	write(fd, buffer, ft_strlen(buffer));
}

int	ft_atoi(const char *nptr)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	while (*nptr && (*nptr == 32 || (*nptr >= 9 && *nptr <= 13)))
		nptr++;
	if (*nptr && (*nptr == '-' || *nptr == '+'))
	{
		if (*nptr == '-')
			sign = sign * -1;
		nptr++;
	}
	while (*nptr && *nptr >= 48 && *nptr <= 57)
	{
		res = (res * 10) + (*nptr - '0');
		nptr++;
	}
	return (res * sign);
}
