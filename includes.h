/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <camille.fabre003@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:15:50 by cafabre           #+#    #+#             */
/*   Updated: 2025/10/10 01:46:31 by cafabre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

size_t	ft_strlen(const char *str);

int		ft_atoi(const char *str);

void	*thread_routine(t_philo *philo);

typedef struct s_philo
{
	pthread_t	thread;
	int			id;
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	size_t		last_meal_time;
	int			number_of_meals;
	int			*dead;
	//mutex
}	t_philo;

typedef struct s_program
{
	//
}	t_program;

#endif