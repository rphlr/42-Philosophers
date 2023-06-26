/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 13:20:35 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/26 13:22:50 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	_die(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	philosophers->utils->should_stop = DEAD;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	usleep(1000);
	print_status(philosophers, RED""_DEAD""RESET);
}

int	can_pick_fork(t_philo *philosophers, pthread_mutex_t *fork_mutex)
{
	int	should_stop_value;

	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	if (!fork_mutex)
		return (0);
	if (!pthread_mutex_lock(fork_mutex))
	{
		if (!should_stop_value && philosophers->running.total_philosophers != 1)
			print_status(philosophers, BLUE""TOOK_FORK""RESET);
		return (1);
	}
	return (0);
}

void	overseer(t_philo *philosophers)
{
	int	should_stop_value;

	while (philosophers->id != 1)
		philosophers = philosophers->next;
	while (1)
	{
		pthread_mutex_lock(&philosophers->utils->stop_mutex);
		should_stop_value = philosophers->utils->should_stop;
		pthread_mutex_unlock(&philosophers->utils->stop_mutex);
		if (should_stop_value == DEAD)
		{
			_die(philosophers);
			break ;
		}
		else if (should_stop_value == EAT_REACHED)
		{
			printf(GREEN""ITALIC""UNDERLINE""STOP_SIMULATION""RESET,
				philosophers->running.eating_count);
			break ;
		}
		philosophers = philosophers->next;
	}
	free_philosophers(philosophers);
}

void	create_thread(t_philo *philosophers)
{
	while (philosophers->id < philosophers->running.total_philosophers)
	{
		pthread_create(&philosophers->thread, NULL, &life_cycle, philosophers);
		philosophers = philosophers->next;
	}
	pthread_create(&philosophers->thread, NULL, &life_cycle, philosophers);
}

t_philo	*last_philo(t_philo *philosophers)
{
	t_philo	*last;

	last = philosophers;
	while (last->next)
		last = last->next;
	last->next = philosophers;
	philosophers->prev = last;
	return (philosophers);
}
