/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:59:02 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 00:49:39 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>

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

static long long	current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

static void	*thread_start(void *arg)
{
	return (thread_routine((t_philo *)arg));
}

static int	init_program(t_program *program)
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

static void	cleanup_program(t_program *program)
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

static int	all_ate_enough(t_program *p)
{
	int	i;

	if (p->must_eat_count <= 0)
		return (0);
	i = 0;
	while (i < p->num_philos)
	{
		if (p->philos[i].times_eaten < p->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

static int	init_philos(t_program *program)
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

static int create_threads(pthread_t **tid_out, t_program *program)
{
	pthread_t	*tid;
	int			i;

	tid = malloc(sizeof(pthread_t) * program->num_philos);
	if (!tid)
		return (1);
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
		usleep(1000);
		i++;
	}
	*tid_out = tid;
	return (0);
}

static void	join_threads(pthread_t *tid, t_program *program)
{
	int	i;

	if (!tid || !program)
		return ;
	i = 0;
	while (i < program->num_philos)
	{
		pthread_join(tid[i], NULL);
		i++;
	}
}

static void monitor_loop(t_program *program)
{
	int			i;
	long long	now;
	long long	last;

	while (1)
	{
		i = 0;
		while (i < program->num_philos)
		{
			now = current_time_ms();
			last = program->philos[i].last_meal_time;
			if (now - last > (long long)program->time_to_die)
			{
				pthread_mutex_lock(program->print_mutex);
				printf("%lld %d died\n", now - program->start_time,
					program->philos[i].id);
				pthread_mutex_unlock(program->print_mutex);
				pthread_mutex_lock(program->death_mutex);
				program->someone_died = 1;
				pthread_mutex_unlock(program->death_mutex);
				return ;
			}
			i++;
		}
		if (all_ate_enough(program))
			return ;
		usleep(1000);
	}
}

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
