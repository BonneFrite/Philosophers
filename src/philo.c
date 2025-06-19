/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:54:49 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 07:47:55 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Alternates which fork philos take first, based on whether
their id is even or odd , to avoid having all philos take on fork
and become unable to find a 2nd one*/
static void	get_forks(t_philos *philos, int nb_philos)
{
	if (philos->id % 2)
	{
		philos->forks[0] = (philos->id + 1) % nb_philos;
		philos->forks[1] = philos->id;
		return ;
	}
	philos->forks[0] = philos->id;
	philos->forks[1] = (philos->id + 1) % nb_philos;
	return ;
}

t_philos **sit_clients(t_waiter *waiter)
{
	int			i;
	int			j;
	t_philos	**philos;

	philos = malloc((waiter->nbphilos) * sizeof(t_philos *));
	if (!philos)
		return (NULL);
	i = -1;
	while (++i < waiter->nbphilos)
	{
		philos[i] = malloc(1 * sizeof(t_philos));
		if (!philos[i])
		{
			j = -1;
			while(++j < i)
				free(philos[j]);
			return (free(philos), NULL);
		}
		philos[i]->waiter = waiter;
		philos[i]->id = i;
		philos[i]->meals = 0;
		get_forks(philos[i], waiter->nbphilos);
	}
	return (philos);
}

bool	philo_starved(t_philos *philo, t_waiter *waiter)
{
	time_t	time;

	time = time_in_ms();
	if ((time - philo->last_meal) >= waiter->ttdie)
	{
		pthread_mutex_lock(&waiter->stop_lock);
		waiter->must_stop = true;
		pthread_mutex_unlock(&waiter->stop_lock);
		display_status(philo, waiter, DEAD);
		pthread_mutex_unlock(&philo->meal_lock);
		return (true);
	}
	return (false);
}

void	*philosopher(void *data)
{
	t_philos	*philo;
	t_waiter	*waiter;

	philo = (t_philos *)data;
	waiter = philo->waiter;
	if (waiter->required_meals == 0)
		return (NULL);
	synchronize_philos(waiter->initial_time, philo);
	if (waiter->ttdie == 0)
		return (NULL);
	if (waiter->nbphilos == 1)
		return (eating_alone(philo, waiter));
	if (philo->id % 2)
		do_some_thinking(philo, waiter, true);
	while (mealtime_over(waiter) == false)
	{
		eat_your_fill(philo);
		take_a_nap(philo);
		do_some_thinking(philo, waiter, false);
	}
	return (NULL);
}
