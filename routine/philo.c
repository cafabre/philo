/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <camille.fabre003@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:16:20 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/29 17:38:59 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	thinking(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(program->death_mutex);
	if (program->someone_died)
	{
		pthread_mutex_unlock(program->death_mutex);
		return ;
	}
	pthread_mutex_unlock(program->death_mutex);
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d is thinking\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
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
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d is sleeping\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
	sleep_ms(program, program->time_to_sleep);
}

void	*handle_one_philo(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(&program->forks[philo->left_fork]);
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d has taken a fork\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
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
	if (program->num_philos == 1)
		return (handle_one_philo(philo, program));
	if (philo->id % 2 == 0)
		usleep(program->time_to_eat * 1000);
	while (1)
	{
		if (check_death(program))
			break ;
		times_eaten = eating_routine(philo, program, &left_fork, &right_fork);
		if (program->must_eat_count > 0 && times_eaten
			>= program->must_eat_count)
			break ;
		sleeping(philo, program);
		thinking(philo, program);
	}
	return (NULL);
}
