/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 02:05:13 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 06:16:03 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void	fill_philosophers_params(t_program *program, char **argv, int argc)
{
	program->num_philos = ft_atoi(argv[1]);
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	program->must_eat_count = -1;
	if (argc == 6)
		program->must_eat_count = ft_atoi(argv[5]);
}

static void	init_philos_data(t_program *program, int i)
{
	program->philos[i].id = i + 1;
	if (i == program->num_philos - 1)
	{
		program->philos[i].left_fork = (i + 1) % program->num_philos;
		program->philos[i].right_fork = i;
	}
	else
	{
		program->philos[i].left_fork = i;
		program->philos[i].right_fork = (i + 1) % program->num_philos;
	}
	program->philos[i].times_eaten = 0;
	program->philos[i].last_meal_time = program->start_time;
	program->philos[i].data = program;
}

int	init_philos(t_program *program)
{
	int			i;
	int			j;

	program->philos = malloc(sizeof(t_philo) * program->num_philos);
	if (!program->philos)
		return (1);
	i = 0;
	while (i < program->num_philos)
	{
		memset(&program->philos[i], 0, sizeof(t_philo));
		if (pthread_mutex_init(&program->philos[i].meal_mutex, NULL) != 0)
		{
			j = 0;
			while (j < i)
				pthread_mutex_destroy(&program->philos[j++].meal_mutex);
			free(program->philos);
			return (1);
		}
		init_philos_data(program, i);
		i++;
	}
	return (0);
}
