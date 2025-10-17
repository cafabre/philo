/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 02:05:13 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 02:06:10 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

int	init_program(t_program *program)
{
	int	i;

	program->start_time = current_time_ms();
	program->someone_died = 0;
	program->print_mutex = malloc(sizeof(pthread_mutex_t));
	program->death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!program->print_mutex || !program->death_mutex)
		return (1);
	if (pthread_mutex_init(program->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(program->death_mutex, NULL) != 0)
		return (1);
	program->forks = malloc(sizeof(pthread_mutex_t) * program->num_philos);
	if (!program->forks)
		return (1);
	i = 0;
	while (i < program->num_philos)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_philos(t_program *program)
{
	int			i;
	t_philo		*philos;

	philos = NULL;
	program->philos = malloc(sizeof(t_philo) * program->num_philos);
	if (!program->philos)
		return (1);
	i = 0;
	while (i < program->num_philos)
	{
		memset(&program->philos[i], 0, sizeof(t_philo));
		philos[i].id = i + 1;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1) % program->num_philos;
		philos[i].times_eaten = 0;
		philos[i].last_meal_time = program->start_time;
		philos[i].data = program;
		i++;
	}
	return (0);
}

void	cleanup_program(t_program *program)
{
	int	i;

	if (!program)
		return ;
	if (program->forks)
	{
		i = 0;
		while (i < program->num_philos)
		{
			pthread_mutex_destroy(&program->forks[i]);
			i++;
		}
		free(program->forks);
		program->forks = NULL;
	}
	if (program->print_mutex)
	{
		pthread_mutex_destroy(program->print_mutex);
		free(program->print_mutex);
		program->print_mutex = NULL;
	}
	if (program->death_mutex)
	{
		pthread_mutex_destroy(program->death_mutex);
		free(program->death_mutex);
		program->death_mutex = NULL;
	}
}
