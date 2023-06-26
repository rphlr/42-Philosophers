/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 13:20:35 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/26 13:20:38 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	perform_life_cycle(t_philo *philosophers)
{
	int	should_stop_value;

	should_stop(philosophers);
	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	if (!should_stop_value)
	{
		_eat(philosophers);
		if (should_stop_value)
			return (1);
		_sleep(philosophers);
		if (should_stop_value)
			return (1);
		_think(philosophers);
		if (should_stop_value)
			return (1);
	}
	else
		return (1);
	return (0);
}

void	*life_cycle(void *p)
{
	t_philo	*philosophers;

	philosophers = (t_philo *) p;
	if (philosophers->state == WAIT_START)
		sleep_ms(philosophers->running.time_to_eat / 2, philosophers);
	while (1)
	{
		if (perform_life_cycle(philosophers))
			break ;
	}
	return (NULL);
}

void	_think(t_philo *philosophers)
{
	int	should_stop_value;

	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	if (!should_stop_value)
	{
		philosophers->state = THINK_;
		print_status(philosophers, CYAN""THINKING""RESET);
	}
}

void	_sleep(t_philo *philosophers)
{
	int	should_stop_value;

	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	if (!should_stop_value)
	{
		philosophers->state = SLEEP_;
		print_status(philosophers, MAGENTA""SLEEPING""RESET);
		sleep_ms(philosophers->running.time_to_sleep, philosophers);
	}
}

void	_eat(t_philo *philosophers)
{
	t_philo	*next;
	int		should_stop_value;

	next = philosophers->next;
	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	if (can_pick_fork(philosophers, &philosophers->fork_mutex)
		&& can_pick_fork(philosophers, &next->fork_mutex))
	{
		if (!should_stop_value)
		{
			philosophers->state = EAT_;
			print_status(philosophers, YELLOW""EATING""RESET);
			philosophers->last_meal_time = get_time();
			if (philosophers->running.eating_count > 0)
				philosophers->n_eat += 1;
			sleep_ms(philosophers->running.time_to_eat, philosophers);
		}
	}
	pthread_mutex_unlock(&philosophers->fork_mutex);
	pthread_mutex_unlock(&next->fork_mutex);
}
