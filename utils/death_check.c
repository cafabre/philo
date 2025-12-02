/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:54:58 by cafabre           #+#    #+#             */
/*   Updated: 2025/12/02 14:37:48 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	check_death(t_program *program)
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
