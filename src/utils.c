/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:08:36 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 03:16:19 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	has_nondigit(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (true);
		i++;
	}
	return (false);
}

bool	mealtime_over(t_waiter *waiter)
{
	bool	over;

	over = false;
	pthread_mutex_lock(&waiter->stop_lock);
	if (waiter->must_stop)
		over = true;
	pthread_mutex_unlock(&waiter->stop_lock);
	return (over);
}

static void	print_message(t_waiter *waiter, t_philos *philo, \
						const char *str)
{
	//printf("%s\n", str);
	printf("%ld %d %s\n", time_in_ms() - waiter->initial_time, philo->id + 1, \
			str);
	return ;
}

void	display_status(t_philos *philo, t_waiter *waiter, int status)
{
	pthread_mutex_lock(&waiter->prog_lock);
	if (mealtime_over(waiter))
	{
		if (status == DEAD)
			print_message(waiter, philo, "died");
		pthread_mutex_unlock(&waiter->prog_lock);
		return ;
	}
	if (status == EATING)
		print_message(waiter, philo, "is eating");
	else if (status == SLEEPING)
		print_message(waiter, philo, "is sleeping");
	else if (status == THINKING)
		print_message(waiter, philo, "is thinking");
	else if (status == HAS_1_FORK || status == HAS_BOTH_FORKS)
		print_message(waiter, philo, "has taken a fork");
	else
		print_message(waiter, philo, "died");
	pthread_mutex_unlock(&waiter->prog_lock);
}

int	ft_atoi(const char *nptr)
{
	long	result;
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
				sign = -1;
		i++;
	}
	result = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + sign * (nptr[i] - '0');
		i++;
		if (result >= 2147483647)
			return (-1);
	}
	return (result);
}
