/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:16:42 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 08:48:35 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	*eating_alone(t_philos *philo, t_waiter *waiter)
{
	pthread_mutex_lock(&waiter->forks_lock[philo->forks[0]]);
	display_status(philo, waiter, HAS_1_FORK);
	try_to_sleep(waiter->ttdie, waiter);
	display_status(philo, waiter, DEAD);
	pthread_mutex_unlock(&waiter->forks_lock[philo->forks[0]]);
	return (NULL);
}

void	eat_your_fill(t_philos *philo)
{
	t_waiter	*waiter;

	waiter = philo->waiter;
	pthread_mutex_lock(&waiter->forks_lock[philo->forks[0]]);
	display_status(philo, waiter, HAS_1_FORK);
	pthread_mutex_lock(&waiter->forks_lock[philo->forks[1]]);
	display_status(philo, waiter, HAS_BOTH_FORKS);
	display_status(philo, waiter, EATING);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = time_in_ms();
	pthread_mutex_unlock(&philo->meal_lock);
	try_to_sleep(waiter->tteat, waiter);
	if (!simulation_over(waiter))
	{
		pthread_mutex_lock(&philo->meal_lock);
		(philo->meals)++;
		pthread_mutex_unlock(&philo->meal_lock);
	}
	pthread_mutex_unlock(&waiter->forks_lock[philo->forks[0]]);
	pthread_mutex_unlock(&waiter->forks_lock[philo->forks[1]]);
}

void	take_a_nap(t_philos *philo)
{
	display_status(philo, philo->waiter, SLEEPING);
	try_to_sleep(philo->waiter->ttsleep, philo->waiter);
}

void	do_some_thinking(t_philos *philo, t_waiter *waiter, bool init)
{
	time_t	reflecting;

		//	reflecting = ((waiter->ttdie - (time_in_ms() - \
	//				philo->last_meal + waiter->tteat)));
	if ((waiter->nbphilos % 2) && (waiter->tteat >= waiter->ttsleep))
		reflecting = (2 * waiter->tteat) - waiter->ttsleep;
	else if (init)
		reflecting = (2 * waiter->tteat) - waiter->ttsleep;
	else
		reflecting = 0;
	if (reflecting < 0)
		reflecting = 0;
	if (!init)
		display_status(philo, waiter, THINKING);
	try_to_sleep(reflecting, waiter);
}

void	*kill_philos(void *data)
{
	t_waiter	*waiter;

	waiter = (t_waiter *)data;
	if (waiter->required_meals == 0)
		return (NULL);
	pthread_mutex_lock(&waiter->stop_lock);
	waiter->must_stop = false;
	pthread_mutex_unlock(&waiter->stop_lock);
	while (time_in_ms() < waiter->initial_time)
		continue;
	usleep(5000);
	while(1)
	{
		if (should_meal_stop(waiter))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
