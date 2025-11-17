/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <camille.fabre003@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:16:20 by cafabre           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/10/29 17:38:59 by cafabre          ###   ########.fr       */
=======
/*   Updated: 2025/11/17 12:28:26 by cafabre          ###   ########.fr       */
>>>>>>> 6be5173 (fixed data races)
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	sleeping(t_philo *philo, t_program *program)
{
	safe_print(program, philo->id, "is sleeping");
	sleep_ms(program, program->time_to_sleep);
}

void	*handle_one_philo(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(&program->forks[philo->left_fork]);
	safe_print(program, philo->id, "has taken a fork");
	sleep_ms(program, program->time_to_die + 10);
	pthread_mutex_unlock(&program->forks[philo->left_fork]);
	return (NULL);
}

static int	check_death(t_program *program)
{
	pthread_mutex_lock(program->death_mutex);
	if (program->someone_died)
	{
		pthread_mutex_unlock(program->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(program->death_mutex);
	return (0);
}

void	*thread_routine(t_philo *philo)
{
	t_program	*program;
	int			left_fork;
	int			right_fork;
	int			times_eaten;

	if (philo == NULL)
		return (NULL);
	program = philo->data;
	// mutex_lock();
	// mutex unlock;
	pthread_mutex_lock(program->death_mutex);
	while (program->start_time == 0 && !program->someone_died)
	{
		pthread_mutex_unlock(program->death_mutex);
		usleep(1000);
		pthread_mutex_lock(program->death_mutex);
	}
	pthread_mutex_unlock(program->death_mutex);
	if ((philo->id % 2) == 0)
		sleep_ms(program, 1);
	if (program->num_philos == 1)
		return (handle_one_philo(philo, program));
	if (philo->id % 2 == 0)
		usleep(program->time_to_eat * 1000);
	while (1)
	{
		if (check_death(program))
			break ;
		}
		pthread_mutex_unlock(program->death_mutex);
		safe_print(program, philo->id, "is thinking");
		times_eaten = eating_routine(philo, program,
				&left_fork, &right_fork);
				/*
		if (program->must_eat_count > 0 && times_eaten
			>= program->must_eat_count)
			break ;*/
		sleeping(philo, program);
		thinking(philo, program);
	}
	return (NULL);
}
