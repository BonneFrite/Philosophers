/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:57:32 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 05:50:29 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	synchronize_philos(time_t start_time, t_philos *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = start_time;
	pthread_mutex_unlock(&philo->meal_lock);
	while (time_in_ms() < start_time)
		continue;
}

time_t	time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	try_to_sleep(time_t needed_sleep, t_waiter *waiter)
{
	time_t	alarm;;

	alarm = time_in_ms() + needed_sleep;
	while (time_in_ms() < alarm)
	{
		if (simulation_over(waiter))
			break;
		usleep(500);
	}
	return ;
}
