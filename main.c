/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <camille.fabre003@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:04:28 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/10 01:48:00 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include <stdio.h>

int	check_args(t_philo philo)
{
	//ft_atoi de tous les args
	
	//check si nombre

	//check si > 0
}

int	main(int argc, char **argv)
{
	pthread_t		tid1;
	pthread_t		tid2;
	unsigned int	*count;

	//check_args

	pthread_create(&tid1, NULL, thread_routine, &count);
	pthread_create(&tid2, NULL, thread_routine, &count);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return (0);
}
