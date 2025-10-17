/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 06:15:50 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 06:17:17 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <stdlib.h>

void	cleanup_philos(t_program *program)
{
	int	i;

	if (!program || !program->philos)
		return ;
	i = 0;
	while (i < program->num_philos)
	{
		pthread_mutex_destroy(&program->philos[i].meal_mutex);
		i++;
	}
	free(program->philos);
	program->philos = NULL;
}

void	destroy_print_death(t_program *program)
{
	if (!program)
		return ;
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
	destroy_print_death(program);
}

void	clean_all(t_program *program)
{
	cleanup_philos(program);
	cleanup_program(program);
	free(program);
}
