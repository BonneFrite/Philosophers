/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:01:57 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 03:46:58 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_mutexes(t_waiter *waiter)
{
	int	i;
	pthread_mutex_t	*forks;

	i = 0;
	forks = malloc(waiter->nbphilos * sizeof(pthread_mutex_t));
	if (!forks)
		return (true);
	waiter->forks_lock = forks;
	while (i < waiter->nbphilos)
	{
		if (pthread_mutex_init(&waiter->philos[i]->meal_lock, 0))
			return (end_of_shift(waiter), MUTEX_ERROR);
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (end_of_shift(waiter), MUTEX_ERROR);
		i++;
	}
	if (pthread_mutex_init(&waiter->stop_lock, 0) != 0)
		return (end_of_shift(waiter), MUTEX_ERROR);
	if (pthread_mutex_init(&waiter->prog_lock, 0) != 0)
		return (end_of_shift(waiter), MUTEX_ERROR);
	return (false);
}

static bool	check_arg_values(int ac, char **av, int *error)
{
	int	i;

	*error = 0;
	i = 1;
	while (!(*error) && i < ac)
	{
		if (has_nondigit(av[i]))
			*error = ARGS_ERROR;
		else if (i == 1 && (ft_atoi(av[i]) <= 0 || ft_atoi(av[i]) > MAX_PHILOS))
			*error = NB_PHILOS;
		else if (i != 1 && (ft_atoi(av[i]) < 0 || ft_atoi(av[i]) >= 2147483647))
			*error = ARGS_ERROR;
		if (*error)
			return (*error);
		i++;
	}
	return (*error);
}

int	check_args(int argc, char **argv, t_waiter *waiter)
{
	int	error;

	if (argc < 5 || argc > 6)
		return(ARGS_FORMAT);
	if (check_arg_values(argc, argv, &error))
		return (error);
	waiter->nbphilos = ft_atoi(argv[1]);
	waiter->ttdie = ft_atoi(argv[2]);
	waiter->tteat = ft_atoi(argv[3]);
	waiter->ttsleep = ft_atoi(argv[4]);
	if (argc == 6)
		waiter->required_meals = ft_atoi(argv[5]);
	else
		waiter->required_meals = -1;
	return (0);
}
