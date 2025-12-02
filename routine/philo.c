/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:16:20 by cafabre           #+#    #+#             */
/*   Updated: 2025/12/02 14:02:44 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	thinking(t_philo *philo, t_program *program)
{
	long long	think_time;

	pthread_mutex_lock(program->death_mutex);
	if (program->someone_died)
	{
		pthread_mutex_unlock(program->death_mutex);
		return ;
	}
	pthread_mutex_unlock(program->death_mutex);
	safe_print(program, philo->id, "is thinking");
	think_time = (program->time_to_die - program->time_to_eat
			- program->time_to_sleep) / 2;
	if (think_time > 0)
		usleep(think_time * 1000);
}

void	sleeping(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(program->death_mutex);
	if (program->someone_died)
	{
		pthread_mutex_unlock(program->death_mutex);
		return ;
	}
	pthread_mutex_unlock(program->death_mutex);
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

void	*thread_routine(t_philo *philo)
{
	t_program	*program;

	if (philo == NULL)
		return (NULL);
	program = philo->data;
	if ((philo->id % 2) == 0)
		usleep(1000);
	if (program->num_philos == 1)
		return (handle_one_philo(philo, program));
	while (1)
	{
		if (check_death(program) == 1)
			break ;
		thinking(philo, program);
		philo->times_eaten = eating_routine(philo, program,
				&philo->left_fork, &philo->right_fork);
		if (program->must_eat_count > 0 && philo->times_eaten
			>= program->must_eat_count)
			break ;
		sleeping(philo, program);
	}
	return (NULL);
}
