/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 13:20:35 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/28 16:45:46 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philo	*create_philos(t_philo *prev, t_running running,
	int i, t_utils *utils)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)ft_calloc(sizeof(t_philo), 1);
	if (!philosophers)
		return (NULL);
	philosophers->id = i + 1;
	philosophers->running = running;
	philosophers->last_meal_time = get_time();
	pthread_mutex_init(&philosophers->time_mtx, NULL);
	pthread_mutex_init(&philosophers->n_eat_mutex, NULL);
	philosophers->utils = utils;
	philosophers->n_eat = 0;
	if (philosophers->id % 2)
		philosophers->state = WAIT_START;
	else
		philosophers->state = WAIT_EAT;
	pthread_mutex_init(&philosophers->fork_mutex, NULL);
	philosophers->prev = prev;
	if (++i < running.total_philosophers)
		philosophers->next = create_philos(philosophers, running, i, utils);
	else
		philosophers->next = NULL;
	return (philosophers);
}

t_running	create_running(int ac, char **av)
{
	t_running	running;

	running.error = 0;
	running.total_philosophers = ft_atoi(av[1]);
	running.time_until_death = ft_atoi(av[2]);
	running.time_to_eat = ft_atoi(av[3]);
	running.time_to_sleep = ft_atoi(av[4]);
	if (running.time_to_sleep < 1 || running.time_to_eat < 1
		|| running.total_philosophers < 1 || running.time_until_death < 1)
		running.error = 1;
	if (ac == 6)
	{
		running.eating_count = ft_atoi(av[5]);
		if (running.eating_count < 1)
			running.error = 1;
	}
	else
		running.eating_count = 0;
	return (running);
}

t_utils	create_utils(void)
{
	t_utils	utils;

	pthread_mutex_init(&utils.stop_mutex, NULL);
	utils.should_stop = 0;
	utils.has_finished = 0;
	utils.start_time = 0;
	utils.time_until_death = 0;
	return (utils);
}

void	free_philosopher(t_philo *philosophers)
{
	pthread_mutex_destroy(&philosophers->fork_mutex);
	if (philosophers->running.total_philosophers > 1)
		pthread_join(philosophers->thread, NULL);
	if (philosophers)
		free(philosophers);
	philosophers = NULL;
}

void	free_philosophers(t_philo *philosophers)
{
	t_philo	*iter;
	int		total_philosophers;

	if (philosophers->running.total_philosophers == 1)
	{
		free_philosopher(philosophers);
		return ;
	}
	total_philosophers = philosophers->running.total_philosophers + 1;
	while (--total_philosophers && philosophers)
	{
		if (philosophers->next)
			iter = philosophers->next;
		free_philosopher(philosophers);
		if (iter)
			philosophers = iter;
	}
}
