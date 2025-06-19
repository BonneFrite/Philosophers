/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:19:42 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 07:48:35 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	simulation_over(t_waiter *waiter)
{
	bool	over;

	over = false;
	pthread_mutex_lock(&waiter->stop_lock);
	if (waiter->must_stop)
		over = true;
	pthread_mutex_unlock(&waiter->stop_lock);
	return (over);
}

int	take_orders(t_waiter *waiter)
{
	waiter->philos = sit_clients(waiter);
	if (!waiter->philos)
		return (MEM_ERROR);
	if (init_mutexes(waiter))
		return (MUTEX_ERROR);
	waiter->must_stop = false;
	return (0);
}

static void	join_threads(t_waiter *waiter, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_join(waiter->philos[j]->thread, NULL);
		j++;
	}
	return ;
}

int	start_simulation(t_waiter *waiter)
{
	int	i;

	i = 0;
	waiter->initial_time = time_in_ms() + waiter->nbphilos * 20;
	while (i < waiter->nbphilos)
	{
		if (pthread_create(&waiter->philos[i]->thread, NULL, &philosopher,
			waiter->philos[i]) != 0)
		{
			join_threads(waiter, i);
			return (end_of_shift(waiter), THREAD_ERROR);
		}
		i++;
	}
	if (waiter->nbphilos > 1)
	{
		if (pthread_create(&waiter->kill_philos, NULL,
			&kill_philos, waiter) != 0)
		{
			join_threads(waiter, waiter->nbphilos);
			return (end_of_shift(waiter), THREAD_ERROR);
		}
	}
	return (false);
}

bool	should_meal_stop(t_waiter *waiter)
{
	int		i;
	int		full_philos;

	i = 0;
	full_philos = 0;
	while (i < waiter->nbphilos)
	{
		pthread_mutex_lock(&waiter->philos[i]->meal_lock);
		if (philo_starved(waiter->philos[i], waiter))
			return (true);
		if (waiter->required_meals >= 0
			&& waiter->philos[i]->meals >= waiter->required_meals)
			full_philos++;
		pthread_mutex_unlock(&waiter->philos[i]->meal_lock);
		i++;
	}
	if (waiter->required_meals >= 0 && full_philos == waiter->nbphilos)
	{
		pthread_mutex_lock(&waiter->stop_lock);
		waiter->must_stop = true;
		pthread_mutex_unlock(&waiter->stop_lock);
		return (true);
	}
	return (false);
}
