/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:40:00 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/29 17:40:00 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <sys/time.h>
#include <unistd.h>

long long	current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

void	sleep_ms(t_program *program, long long ms)
{
	long long	start;
	long long	now;

	start = current_time_ms();
	while (1)
	{
		pthread_mutex_lock(program->death_mutex);
		if (program->someone_died)
		{
			pthread_mutex_unlock(program->death_mutex);
			break ;
		}
		pthread_mutex_unlock(program->death_mutex);
		now = current_time_ms();
		if (now - start >= ms)
			break ;
		usleep(1000);
	}
}
