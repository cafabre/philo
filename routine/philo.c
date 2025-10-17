/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:16:20 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 09:20:18 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	thinking(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d is thinking\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
}

void	sleeping(t_philo *philo, t_program *program)
{
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

void	*thread_routine(t_philo *philo)
{
	t_program	*program;
	int			left_fork;
	int			right_fork;
	int			times_eaten;

	if (philo == NULL)
		return (NULL);
	program = philo->data;
	if ((philo->id % 2) == 0)
		sleep_ms(program, 1);
	if (program->num_philos == 1)
		return (handle_one_philo(philo, program));
	while (1)
	{
		pthread_mutex_lock(program->death_mutex);
		if (program->someone_died)
		{
			pthread_mutex_unlock(program->death_mutex);
			break ;
		}
		pthread_mutex_unlock(program->death_mutex);
		thinking(philo, program);
		times_eaten = eating_routine(philo, program,
				&left_fork, &right_fork);
		if (program->must_eat_count > 0 && times_eaten
			>= program->must_eat_count)
			break ;
		sleeping(philo, program);
	}
	return (NULL);
}
