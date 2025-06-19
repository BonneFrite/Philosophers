/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpozo <vpozo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:06:28 by vpozo             #+#    #+#             */
/*   Updated: 2025/06/19 01:56:39 by vpozo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(int error)
{
	if (error == ARGS_FORMAT)
		printf("%s%s%s\n", FORMAT_STR1, FORMAT_STR2, FORMAT_STR3);
	else if (error == MEM_ERROR)
		write(STDOUT_FILENO, "Insufficient memory", 20);
	else if (error == ARGS_ERROR)
	{
		printf("Args must be : a valid nb of philosophers, time to die, ea");
		printf("t and sleep, and optionally amount of meals\n");
	}
	else if (error == NB_PHILOS)
	{
		printf("Must input a number of philos between 1 and %d\n", MAX_PHILOS);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_waiter	waiter;
	int			error;

	error = check_args(argc, argv, &waiter);
	if (error)
		return (ft_error(error));
	if (take_orders(&waiter))
		return (ft_error(MEM_ERROR));
	if (start_simulation(&waiter))
		return (ft_error(THREAD_ERROR));
	stop_everything(&waiter);
	return (EXIT_SUCCESS);
}
