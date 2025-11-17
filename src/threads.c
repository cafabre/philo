/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 02:06:28 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/20 17:42:27 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	*thread_start(void *arg)
{
	return (thread_routine((t_philo *)arg));
}

int	create_threads(pthread_t **tid_out, t_program *program)
{
	pthread_t	*tid;
	int			i;

	tid = malloc(sizeof(pthread_t) * program->num_philos);
	if (!tid)
		return (-1);
	i = 0;
	while (i < program->num_philos)
	{
		if (pthread_create(&tid[i], NULL, thread_start,
				&program->philos[i]) != 0)
		{
			perror("pthread_create");
			program->someone_died = 1;
			break ;
		}
		i++;
	}
	// programme start time, commence ici programme->starttime = current ime ms;
	//program->start_time = current_time_ms(); 
	*tid_out = tid;
	return (i);
}

void	join_threads(pthread_t *tid, t_program *program, int threads_created)
{
	int	i;

	if (!tid || !program)
		return ;
	i = 0;
	while (i < program->num_philos && i < threads_created)
	{
		pthread_join(tid[i], NULL);
		i++;
	}
}
