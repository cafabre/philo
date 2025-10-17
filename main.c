/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:59:02 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 05:04:50 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	pthread_t		*tid;
	t_program		*program;
	int				threads_created;

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
	fill_philosophers_params(program, argv, argc);
	if (init_program(program) != 0 || init_philos(program) != 0)
	{
		printf("Error: initialization failed\n");
		clean_all(program);
		return (1);
	}
	threads_created = create_threads(&tid, program);
	if (threads_created < 0)
	{
		printf("Error: thread creation failed\n");
		clean_all(program);
		return (1);
	}
	monitor_loop(program);
	join_threads(tid, program, threads_created);
	if (tid)
		free(tid);
	clean_all(program);
	return (0);
}
