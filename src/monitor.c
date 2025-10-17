/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 02:07:03 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/17 02:08:12 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

long long	current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

int	all_ate_enough(t_program *p)
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

void monitor_loop(t_program *program)
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
