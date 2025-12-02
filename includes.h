/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:15:50 by cafabre           #+#    #+#             */
/*   Updated: 2025/12/02 13:58:33 by cafabre          ###   ########.fr       */
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

typedef struct s_program	t_program;

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	int				id;
	int				left_fork;
	int				right_fork;
	long long		last_meal_time;
	int				times_eaten;
	t_program		*data;
}	t_philo;

typedef struct s_program
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat_count;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*death_mutex;
	int				someone_died;
	t_philo			*philos;
}	t_program;

/********** utils.c **********/
size_t		ft_strlen(const char *str);
int			ft_atoi(const char *str);

/********** eating.c **********/
int			eating_routine(t_philo *philo, t_program *program,
				int *left_fork, int *right_fork);

/********** philo.c **********/
void		*thread_routine(t_philo *philo);

/********** args.c **********/
int			check_args(char **argv);

/********** init_philos.c **********/
void		fill_philosophers_params(t_program *program, char **argv, int argc);
int			init_philos(t_program *program);

/********** init_prog.c **********/
int			init_program(t_program *program);

/********** cleanup.c **********/
void		destroy_print_death(t_program *program);
void		cleanup_philos(t_program *program);
void		cleanup_program(t_program *program);
void		clean_all(t_program *program);

/********** monitor.c **********/
long long	current_time_ms(void);
void		sleep_ms(t_program *program, long long ms);
int			all_ate_enough(t_program *p);
void		monitor_loop(t_program *program);

/********** threads.c **********/
void		*thread_start(void *arg);
int			create_threads(pthread_t **tid_out, t_program *program);
void		join_threads(pthread_t *tid, t_program *program,
				int threads_created);

/********** print_utils.c **********/
void		safe_print(t_program *program, int id, const char *msg);

/********** check_death.c.c **********/
int			check_death(t_program *program);

#endif