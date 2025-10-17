/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:16:20 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 07:09:09 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	thinking(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d is thinking\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
}

void	taking_forks(t_philo *philo, t_program *program,
		int *left_fork, int *right_fork)
{
	*left_fork = philo->left_fork;
	*right_fork = philo->right_fork;
	if (left_fork < right_fork)
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

void	announce_fork_taken(t_philo *philo, t_program *program, int *fork)
{
	pthread_mutex_lock(&program->forks[*fork]);
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d has taken a fork\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
}

void	eating(t_philo *philo, t_program *program)
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

void	sleeping(t_philo *philo, t_program *program)
{
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d is sleeping\n",
		current_time_ms() - program->start_time, philo->id);
	pthread_mutex_unlock(program->print_mutex);
	sleep_ms(program, program->time_to_sleep);
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
	while (1)
	{
		//1 : verifier si un philosophe est mort
		pthread_mutex_lock(program->death_mutex);
		if (program->someone_died)
		{
			pthread_mutex_unlock(program->death_mutex);
			break ;
		}
		pthread_mutex_unlock(program->death_mutex);
		//2 : think
		thinking(philo, program);
		//3 : take forks (ordre fixe)
		taking_forks(philo, program, &left_fork, &right_fork);
		announce_fork_taken(philo, program, &left_fork);
		announce_fork_taken(philo, program, &right_fork);
		//4 : eat
		eating(philo, program);
		times_eaten = philo->times_eaten;
		pthread_mutex_unlock(&philo->meal_mutex);
		//5 : put down forks (inverse ordre)
		pthread_mutex_unlock(&program->forks[right_fork]);
		pthread_mutex_unlock(&program->forks[left_fork]);
		//6 : si must_eat_count definit et atteint, break
		if (program->must_eat_count > 0 && times_eaten
			>= program->must_eat_count)
			break ;
		//7 : dormir
		sleeping(philo, program);
	}
	return (NULL);
}
