/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_prog.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 04:40:09 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 06:17:02 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static int	alloc_print_death(t_program *program)
{
	program->print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!program->print_mutex)
		return (1);
	program->death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!program->death_mutex)
	{
		free(program->print_mutex);
		program->print_mutex = NULL;
		return (1);
	}
	return (0);
}

static int	init_print_death(t_program *program)
{
	int	ret;

	ret = pthread_mutex_init(program->print_mutex, NULL);
	if (ret != 0)
	{
		free(program->print_mutex);
		program->print_mutex = NULL;
		free(program->death_mutex);
		program->death_mutex = NULL;
		return (1);
	}
	ret = pthread_mutex_init(program->death_mutex, NULL);
	if (ret != 0)
	{
		pthread_mutex_destroy(program->print_mutex);
		free(program->print_mutex);
		program->print_mutex = NULL;
		free(program->death_mutex);
		program->death_mutex = NULL;
		return (1);
	}
	return (0);
}

static int	init_forks(t_program *program)
{
	int	i;
	int	ret;

	program->forks = malloc(sizeof(pthread_mutex_t) * program->num_philos);
	if (!program->forks)
		return (1);
	i = 0;
	while (i < program->num_philos)
	{
		ret = pthread_mutex_init(&program->forks[i], NULL);
		if (ret != 0)
		{
			while (--i > 0)
				pthread_mutex_destroy(&program->forks[i]);
			free(program->forks);
			program->forks = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_program(t_program *program)
{
	program->start_time = current_time_ms();
	program->someone_died = 0;
	if (alloc_print_death(program) != 0)
		return (1);
	if (init_print_death(program) != 0)
	{
		destroy_print_death(program);
		return (1);
	}
	if (init_forks(program) != 0)
	{
		destroy_print_death(program);
		return (1);
	}
	return (0);
}
