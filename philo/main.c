/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 12:35:30 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/26 15:18:44 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf(RED""BOLD""UNDERLINE""USAGE""RESET);
		return (0);
	}
	init(ac, av);
	return (0);
}

int	init_structures(t_running *running, t_utils *utils, int ac, char **av)
{
	*running = create_running(ac, av);
	if (running->error)
	{
		printf(RED""BOLD""UNDERLINE""ERR_INVALID_ARGS""RESET);
		return (1);
	}
	*utils = create_utils();
	if (running->total_philosophers >= 87353)
	{
		printf(RED""BOLD""UNDERLINE""MAX_THREAD""RESET);
		return (1);
	}
	return (0);
}

t_philo	*create_and_setup_philosophers(t_running *running, t_utils *utils)
{
	t_philo	*philosophers;

	philosophers = create_philos(NULL, *running, 0, utils);
	philosophers = last_philo(philosophers);
	philosophers->utils->start_time = get_time();
	while (philosophers->id != 1)
		philosophers = philosophers->next;
	create_thread(philosophers);
	return (philosophers);
}

void	handle_single_philosopher(t_philo *philosophers, t_running *running)
{
	print_status(philosophers, BLUE""TOOK_FORK""RESET);
	sleep_ms(running->time_until_death, philosophers);
	_die(philosophers);
}

void	init(int ac, char **av)
{
	t_philo		*philosophers;
	t_running	running;
	t_utils		utils;

	if (init_structures(&running, &utils, ac, av))
		return ;
	philosophers = create_and_setup_philosophers(&running, &utils);
	if (running.total_philosophers == 1)
		handle_single_philosopher(philosophers, &running);
	else
		overseer(philosophers);
}
