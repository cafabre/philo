/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 08:36:19 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 09:03:20 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

static void	taking_forks(t_philo *philo, t_program *program,
	int *left_fork, int *right_fork)
{
	*left_fork = philo->left_fork;
	*right_fork = philo->right_fork;
	if (*left_fork < *right_fork)
	{
		pthread_mutex_lock(&program->forks[*left_fork]);
		pthread_mutex_lock(&program->forks[*right_fork]);
	}
	else
	{
		pthread_mutex_lock(&program->forks[*right_fork]);
		pthread_mutex_lock(&program->forks[*left_fork]);
	}
}

static void	announce_fork_taken(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d has taken a fork\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
}

static void	eating(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = current_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d is eating\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
	sleep_ms(program, program->time_to_eat);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->times_eaten += 1;
}

int	eating_routine(t_philo *philo, t_program *program,
	int *left_fork, int *right_fork)
{
	int	times_eaten;

	taking_forks(philo, program, left_fork, right_fork);
	announce_fork_taken(philo, program);
	announce_fork_taken(philo, program);
	eating(philo, program);
	times_eaten = philo->times_eaten;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(&program->forks[*right_fork]);
	pthread_mutex_unlock(&program->forks[*left_fork]);
	return (times_eaten);
}
