/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 12:54:47 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 02:27:58 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutex_destroyer(t_waiter *waiter)
{
	int	i;

	pthread_mutex_destroy(&waiter->stop_lock);
	pthread_mutex_destroy(&waiter->prog_lock);
	i = 0;
	while(i < waiter->nbphilos)
	{
		pthread_mutex_destroy(&waiter->philos[i]->meal_lock);
		pthread_mutex_destroy(&waiter->forks_lock[i]);
		i++;
	}
}

void	end_of_shift(t_waiter *waiter)
{
	int	i;

	if (!waiter)
		return ;
	i = 0;
	while (waiter->philos && i < waiter->nbphilos)
	{
		free(waiter->philos[i]);
		i++;
	}
	free(waiter->philos);
	free(waiter->forks_lock);
	return ;
}

void	stop_everything(t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < waiter->nbphilos)
	{
		pthread_join(waiter->philos[i]->thread, NULL);
		i++;
	}
	if (waiter->nbphilos > 1)
		pthread_join(waiter->kill_philos, NULL);
	mutex_destroyer(waiter);
	end_of_shift(waiter);
}
