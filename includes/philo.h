/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:54:47 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 07:48:11 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILOS 200

# define FORMAT_STR1 "Prompt the program with format : ./philo number"
# define FORMAT_STR2 "_of_philosophers time_to_die time_to_eat time_to_sleep"
# define FORMAT_STR3 "[number_of_times_each_philosopher_must_eat]"

typedef struct s_waiter	t_waiter;

typedef struct s_philos {
	int				id;
	int				meals;
	int				forks[2];
	pthread_mutex_t	meal_lock;
	pthread_t		thread;
	time_t			last_meal;
	t_waiter		*waiter;
} t_philos;

typedef struct s_waiter {
	int				nbphilos;
	int				required_meals;
	bool			must_stop;
	time_t			initial_time;
	time_t			ttdie;
	time_t			tteat;
	time_t			ttsleep;
	pthread_t		kill_philos;
	t_philos		**philos;
	pthread_mutex_t	prog_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	*forks_lock;
} t_waiter;

typedef enum e_infos
{
	DEAD,
	SLEEPING,
	THINKING,
	EATING,
	HAS_1_FORK,
	HAS_BOTH_FORKS,
}	t_infos;

typedef enum e_errors
{
	OK,
	ARGS_FORMAT,
	ARGS_ERROR,
	MEM_ERROR,
	NB_PHILOS,
	MUTEX_ERROR,
	THREAD_ERROR,
}	t_errors;

int			check_args(int argc, char **argv, t_waiter *waiter);
int			init_table(t_waiter waiter, t_philos ***philos, int ac, char **av);
int			start_simulation(t_waiter *waiter);
bool		simulation_over(t_waiter *waiter);
int			take_orders(t_waiter *waiter);
t_philos	**sit_clients(t_waiter *waiter);
time_t		time_in_ms(void);
int			ft_atoi(const char *nptr);
void		try_to_sleep(time_t needed_sleep, t_waiter *waiter);
void		end_of_shift(t_waiter *waiter);
void		mutex_destroyer(t_waiter *waiter);
bool		has_nondigit(const char *str);
bool		init_mutexes(t_waiter *waiter);
void		*philosopher(void *data);
void		*kill_philos(void *data);
void		synchronize_philos(time_t start_time, t_philos *philo);
void		display_status(t_philos *philo, t_waiter *waiter, int status);
bool		mealtime_over(t_waiter *waiter);
void		do_some_thinking(t_philos *philo, t_waiter *waiter, bool init);
void		take_a_nap(t_philos *philo);
void		eat_your_fill(t_philos *philo);
bool		should_meal_stop(t_waiter *waiter);
bool		philo_starved(t_philos *philo, t_waiter *waiter);
void		stop_everything(t_waiter *waiter);
int			*eating_alone(t_philos *philo, t_waiter *waiter);
int			ft_error(int error);

#endif
