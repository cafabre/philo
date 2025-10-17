/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:59:02 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 02:07:34 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>

int	main(int argc, char **argv)
{
	pthread_t		*tid;
	t_program		*program;

	tid = NULL;
	if (!check_args(argv) || (argc != 5 && argc != 6))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	program = malloc(sizeof(t_program));
	if (!program)
		return (1);
	memset(program, 0, sizeof(t_program));
	program->num_philos = ft_atoi(argv[1]);
	program->time_to_die = (size_t)ft_atoi(argv[2]);
	program->time_to_eat = (size_t)ft_atoi(argv[3]);
	program->time_to_sleep = (size_t)ft_atoi(argv[4]);
	program->must_eat_count = -1;
	if (argc == 6)
		program->must_eat_count = ft_atoi(argv[5]);
	if (init_program(program) != 0)
	{
		printf("Error: initialization failed\n");
		cleanup_program(program);
		free(program);
		return (1);
	}
	if (init_philos(program) != 0)
	{
		printf("Error: initialization failed\n");
		cleanup_program(program);
		free(program->philos);
		free(program);
		return (1);
	}
	if (create_threads(&tid, program) != 0)
	{
		printf("Error: thread creation failed\n");
		cleanup_program(program);
		free(program->philos);
		free(program);
		return (1);
	}
	monitor_loop(program);
	join_threads(tid, program);
	if (tid)
		free(tid);
	if (program->philos)
		free(program->philos);
	cleanup_program(program);
	free(program);
	return (0);
}
