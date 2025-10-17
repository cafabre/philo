/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 02:04:53 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 02:05:02 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

int	check_args(char **argv)
{
	int			i;
	const char	*str;

	if (!argv || !argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		str = argv[i];
		if (*str == '+' || *str == '-')
			str++;
		if (*str == '\0')
			return (0);
		while (*str)
		{
			if (*str < '0' || *str > '9')
				return (0);
			str++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
}
