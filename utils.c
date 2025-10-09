/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <camille.fabre003@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:54:00 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/10 01:40:57 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static long long int	ft_check_overflow(long long int res, int n, int sign)
{
	long long int	long_max;
	long long int	long_min;

	long_max = 9223372036854775807LL;
	long_min = -9223372036854775807LL - 1;
	if (res > (long_max - n) / 10)
	{
		if (sign == 1)
			return (long_max);
		else
			return (long_min);
	}
	return (-1);
}

int	ft_atoi(const char *str)
{
	long long int	result;
	int				sign;
	int				i;
	int				n;

	result = 0;
	sign = 1;
	i = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		n = str[i] - '0';
		if (ft_check_overflow(result, n, sign) != -1)
			return (ft_check_overflow(result, n, sign));
		result = 10 * result + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
