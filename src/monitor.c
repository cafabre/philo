/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 02:07:03 by cafabre           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/10/29 17:40:00 by cafabre          ###   ########.fr       */
=======
/*   Updated: 2025/11/17 13:12:32 by cafabre          ###   ########.fr       */
>>>>>>> 6be5173 (fixed data races)
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <unistd.h>
#include <stdio.h>

int	all_ate_enough(t_program *program)
{
	int	i;
	int	times_eaten;

	if (program->must_eat_count <= 0)
		return (0);
	i = 0;
	while (i < program->num_philos)
	{
		pthread_mutex_lock(&program->philos[i].meal_mutex);
		times_eaten = program->philos[i].times_eaten;
		pthread_mutex_unlock(&program->philos[i].meal_mutex);
		printf("Philo %d has eaten %d times\n", program->philos[i].id, times_eaten);
		if (times_eaten < program->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

static void	handle_death(t_program *program, int i, long long now)
{
	pthread_mutex_lock(program->death_mutex);
	program->someone_died = 1;
	pthread_mutex_unlock(program->death_mutex);
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d died\n", now - program->start_time, program->philos[i].id);
	pthread_mutex_unlock(program->print_mutex);
}

static int	check_philo_death(t_program *program, int i)
{
	long long	now;
	long long	last;
	int	times_eaten;
	int check;


	now = current_time_ms();
	pthread_mutex_lock(&program->philos[i].meal_mutex);
	last = (long long)program->philos[i].last_meal_time;
	pthread_mutex_unlock(&program->philos[i].meal_mutex);
	if (now - last > (long long)program->time_to_die)
	{
		handle_death(program, i, now);
		return (1);
	}
	return (0);
}

void	monitor_loop(t_program *program)
{
	int	i;

	while (1)
	{
		i = 0;
		check = 0;
		while (i < program->num_philos)
		{
			if (check_philo_death(program, i))
				return ;
			}

			pthread_mutex_lock(&program->philos[i].meal_mutex);
			times_eaten = program->philos[i].times_eaten;
			pthread_mutex_unlock(&program->philos[i].meal_mutex);
			if (times_eaten >= program->must_eat_count)
				check++;
			if (check >= program->num_philos)
			{
				pthread_mutex_lock(program->death_mutex);
				program->someone_died = 1;
				pthread_mutex_unlock(program->death_mutex);
				return;
			}
			
			i++;
		}
	}
}
