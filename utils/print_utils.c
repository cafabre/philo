/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 03:41:02 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/18 04:58:27 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include <stdio.h>

void	safe_print(t_program *program,  int id, const char *msg)
{
	if (!program || !msg)
		return ;
	pthread_mutex_lock(program->death_mutex);
	if (program->someone_died)
	{
		pthread_mutex_unlock(program->death_mutex);
		return;
	}
	pthread_mutex_unlock(program->death_mutex);
	pthread_mutex_lock(program->print_mutex);
	printf("%lld %d %s\n", current_time_ms()
		- program->start_time, id, msg);
	pthread_mutex_unlock(program->print_mutex);
}
