/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 08:36:19 by cafabre           #+#    #+#             */
/*   Updated: 2025/11/17 14:46:43 by cafabre          ###   ########.fr       */
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

static void	eating(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(program->death_mutex);
	if (program->someone_died)
	{
		pthread_mutex_unlock(program->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(program->death_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = current_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	safe_print(program, philo->id, "is eating");
	sleep_ms(program, program->time_to_eat);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->times_eaten += 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

static void	release_forks(t_program *program, int left, int right)
{
	pthread_mutex_unlock(&program->forks[right]);
	pthread_mutex_unlock(&program->forks[left]);
}

int	eating_routine(t_philo *philo, t_program *program,
	int *left_fork, int *right_fork)
{
	int	times_eaten;

	taking_forks(philo, program, left_fork, right_fork);
	safe_print(program, philo->id, "has taken a fork");
	safe_print(program, philo->id, "has taken a fork");
	eating(philo, program);
	times_eaten = philo->times_eaten;
	pthread_mutex_unlock(&philo->meal_mutex);
	release_forks(program, *left_fork, *right_fork);
	return (times_eaten);
}
