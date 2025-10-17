/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_prog.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 04:40:09 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 05:16:30 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void	destroy_and_free(t_program *program)
{
	pthread_mutex_destroy(program->print_mutex);
	free(program->print_mutex);
	pthread_mutex_destroy(program->death_mutex);
	free(program->death_mutex);
}

int	init_program(t_program *program)
{
	int	i;
	int	j;

	program->start_time = current_time_ms();
	program->someone_died = 0;
	program->print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!program->print_mutex)
		return (1);
	program->death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!program->death_mutex)
	{
		free(program->print_mutex);
		return (1);
	}
	if (pthread_mutex_init(program->print_mutex, NULL) != 0)
	{
		free(program->print_mutex);
		free(program->death_mutex);
		return (1);
	}
	if (pthread_mutex_init(program->death_mutex, NULL) != 0)
		return (1);
	program->forks = malloc(sizeof(pthread_mutex_t) * program->num_philos);
	if (!program->forks)
	{
		destroy_and_free(program);
		return (1);
	}
	i = 0;
	while (i < program->num_philos)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
		{
			j = 0;
			while (j < i)
				pthread_mutex_destroy(&program->forks[j++]);
			free(program->forks);
			destroy_and_free(program);
			return (1);
		}
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
			pthread_mutex_destroy(&program->forks[i++]);
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

void	clean_all(t_program *program)
{
	cleanup_philos(program);
	cleanup_program(program);
	free(program);
}
